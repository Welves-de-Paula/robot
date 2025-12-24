<template>
  <div class="joystick-container">
    <div ref="joystickArea" class="joystick-area" @mousedown="handleStart" @touchstart="handleStart">
      <div ref="joystickHandle" class="joystick-handle" :style="handleStyle"></div>
    </div>
    <div class="joystick-info">
      <p>{{ label }}</p>
      <p>X: {{ position.x.toFixed(2) }}</p>
      <p>Y: {{ position.y.toFixed(2) }}</p>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, onMounted, onUnmounted } from 'vue'

interface Props {
  label?: string
}

const props = withDefaults(defineProps<Props>(), {
  label: 'Joystick'
})

const emit = defineEmits<{
  (e: 'move', position: { x: number, y: number }): void
}>()

const joystickArea = ref<HTMLElement | null>(null)
const joystickHandle = ref<HTMLElement | null>(null)
const isDragging = ref(false)
const position = ref({ x: 0, y: 0 })
const maxDistance = 50

const handleStyle = computed(() => ({
  transform: `translate(${position.value.x}px, ${position.value.y}px)`
}))

const handleStart = (event: MouseEvent | TouchEvent) => {
  event.preventDefault()
  isDragging.value = true
  handleMove(event)
}

const handleMove = (event: MouseEvent | TouchEvent) => {
  if (!isDragging.value || !joystickArea.value) return

  const rect = joystickArea.value.getBoundingClientRect()
  const centerX = rect.left + rect.width / 2
  const centerY = rect.top + rect.height / 2

  let clientX: number
  let clientY: number

  if (event instanceof MouseEvent) {
    clientX = event.clientX
    clientY = event.clientY
  } else {
    clientX = event.touches[0].clientX
    clientY = event.touches[0].clientY
  }

  let deltaX = clientX - centerX
  let deltaY = clientY - centerY

  const distance = Math.sqrt(deltaX * deltaX + deltaY * deltaY)

  if (distance > maxDistance) {
    const angle = Math.atan2(deltaY, deltaX)
    deltaX = Math.cos(angle) * maxDistance
    deltaY = Math.sin(angle) * maxDistance
  }

  position.value = { x: deltaX, y: deltaY }

  // Normalize to -1 to 1 range
  const normalizedX = deltaX / maxDistance
  const normalizedY = deltaY / maxDistance

  emit('move', { x: normalizedX, y: normalizedY })
}

const handleEnd = () => {
  isDragging.value = false
  position.value = { x: 0, y: 0 }
  emit('move', { x: 0, y: 0 })
}

onMounted(() => {
  document.addEventListener('mousemove', handleMove)
  document.addEventListener('mouseup', handleEnd)
  document.addEventListener('touchmove', handleMove)
  document.addEventListener('touchend', handleEnd)
})

onUnmounted(() => {
  document.removeEventListener('mousemove', handleMove)
  document.removeEventListener('mouseup', handleEnd)
  document.removeEventListener('touchmove', handleMove)
  document.removeEventListener('touchend', handleEnd)
})
</script>

<style scoped>
.joystick-container {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 1rem;
}

.joystick-area {
  width: 150px;
  height: 150px;
  background: linear-gradient(145deg, #2a2a2a, #1a1a1a);
  border-radius: 50%;
  position: relative;
  cursor: pointer;
  box-shadow:
    inset 0 0 15px rgba(0, 0, 0, 0.5),
    0 4px 10px rgba(0, 0, 0, 0.3);
  border: 3px solid #333;
}

.joystick-handle {
  width: 50px;
  height: 50px;
  background: linear-gradient(145deg, #4a9eff, #2171e0);
  border-radius: 50%;
  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
  transition: transform 0.1s ease-out;
  box-shadow:
    0 4px 8px rgba(0, 0, 0, 0.4),
    inset 0 2px 4px rgba(255, 255, 255, 0.3);
  border: 2px solid #1a5bb8;
}

.joystick-area:active .joystick-handle {
  background: linear-gradient(145deg, #5aafff, #3181f0);
}

.joystick-info {
  text-align: center;
  font-family: 'Courier New', monospace;
  color: #4a9eff;
}

.joystick-info p {
  margin: 0.25rem 0;
  font-size: 0.9rem;
}

/* Responsividade para telas pequenas */
@media (max-width: 768px) {
  .joystick-area {
    width: 120px;
    height: 120px;
  }

  .joystick-handle {
    width: 40px;
    height: 40px;
  }

  .joystick-info p {
    font-size: 0.8rem;
  }
}

/* Otimização para modo horizontal em dispositivos móveis */
@media (max-width: 926px) and (orientation: landscape) {
  .joystick-container {
    gap: 0.5rem;
  }

  .joystick-area {
    width: 100px;
    height: 100px;
  }

  .joystick-handle {
    width: 35px;
    height: 35px;
  }

  .joystick-info {
    font-size: 0.75rem;
  }

  .joystick-info p {
    margin: 0.1rem 0;
    font-size: 0.7rem;
  }
}

/* Telas muito pequenas em horizontal */
@media (max-height: 500px) and (orientation: landscape) {
  .joystick-area {
    width: 80px;
    height: 80px;
  }

  .joystick-handle {
    width: 28px;
    height: 28px;
  }

  .joystick-info p {
    font-size: 0.65rem;
  }
}
</style>
