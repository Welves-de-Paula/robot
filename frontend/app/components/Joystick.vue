<template>
  <div class="joystick-container">
    <div ref="joystickArea" class="joystick-area" @mousedown="handleStart" @touchstart="handleStart">
      <div ref="joystickHandle" class="joystick-handle" :style="handleStyle"></div>
    </div>
    <div class="joystick-info">
      <p>{{ label }}</p>
      <p>X: {{ normalizedPosition.x.toFixed(2) }}</p>
      <p>Y: {{ normalizedPosition.y.toFixed(2) }}</p>
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
const normalizedPosition = ref({ x: 0, y: 0 })
const deadZone = 0.05 // 5% dead zone no centro
const activeTouchId = ref<number | null>(null) // Rastrear qual toque está ativo

const handleStyle = computed(() => ({
  transform: `translate(calc(-50% + ${position.value.x}px), calc(-50% + ${position.value.y}px))`
}))

const handleStart = (event: MouseEvent | TouchEvent) => {
  event.preventDefault()
  event.stopPropagation() // Impedir que o evento se propague para outros componentes

  if (event instanceof TouchEvent && event.touches.length > 0) {
    // Para touch, encontrar o toque que está dentro da área do joystick
    const rect = joystickArea.value?.getBoundingClientRect()
    if (!rect) return

    for (const touch of Array.from(event.touches)) {
      const isInside = touch.clientX >= rect.left && touch.clientX <= rect.right &&
        touch.clientY >= rect.top && touch.clientY <= rect.bottom

      if (isInside && activeTouchId.value === null) {
        activeTouchId.value = touch.identifier
        break
      }
    }
  }

  isDragging.value = true
  handleMove(event)
}

const handleMove = (event: MouseEvent | TouchEvent) => {
  if (!isDragging.value || !joystickArea.value) return

  // Para mouse, processar normalmente
  if (event instanceof MouseEvent) {
    const rect = joystickArea.value.getBoundingClientRect()
    const centerX = rect.left + rect.width / 2
    const centerY = rect.top + rect.height / 2
    const maxDistance = Math.min(rect.width, rect.height) / 3

    processMovement(event.clientX, event.clientY, centerX, centerY, maxDistance)
    return
  }

  // Para TouchEvent, só processar se for o toque correto deste joystick
  if (activeTouchId.value === null) return

  const touch = Array.from(event.touches).find(t => t.identifier === activeTouchId.value)
  if (!touch) return // Se não encontrar o toque correto, ignorar

  const rect = joystickArea.value.getBoundingClientRect()
  const centerX = rect.left + rect.width / 2
  const centerY = rect.top + rect.height / 2
  const maxDistance = Math.min(rect.width, rect.height) / 3

  processMovement(touch.clientX, touch.clientY, centerX, centerY, maxDistance)
}

const processMovement = (clientX: number, clientY: number, centerX: number, centerY: number, maxDistance: number) => {
  let deltaX = clientX - centerX
  let deltaY = -(clientY - centerY) // INVERTIDO: negativo para que "para cima" seja positivo

  const distance = Math.sqrt(deltaX * deltaX + deltaY * deltaY)

  // Limitar ao raio máximo
  if (distance > maxDistance) {
    const angle = Math.atan2(deltaY, deltaX)
    deltaX = Math.cos(angle) * maxDistance
    deltaY = Math.sin(angle) * maxDistance
  }

  // Atualizar posição visual (Y invertido de volta para display)
  position.value = { x: deltaX, y: -deltaY }

  // Normalizar para -1 a 1
  let normalizedX = deltaX / maxDistance
  let normalizedY = deltaY / maxDistance

  // Aplicar dead zone
  const normalizedDistance = Math.sqrt(normalizedX * normalizedX + normalizedY * normalizedY)

  if (normalizedDistance < deadZone) {
    normalizedX = 0
    normalizedY = 0
  } else {
    // Reescalar para manter a transição suave após o dead zone
    const scale = (normalizedDistance - deadZone) / (1 - deadZone)
    const angle = Math.atan2(normalizedY, normalizedX)
    normalizedX = Math.cos(angle) * scale
    normalizedY = Math.sin(angle) * scale
  }

  // Garantir que os valores estão entre -1 e 1
  normalizedX = Math.max(-1, Math.min(1, normalizedX))
  normalizedY = Math.max(-1, Math.min(1, normalizedY))

  normalizedPosition.value = { x: normalizedX, y: normalizedY }
  emit('move', { x: normalizedX, y: normalizedY })
}

const handleEnd = (event?: MouseEvent | TouchEvent) => {
  // Para TouchEvent, verificar se o toque que terminou é o nosso
  if (event instanceof TouchEvent) {
    const endedTouch = Array.from(event.changedTouches).find(t => t.identifier === activeTouchId.value)
    if (!endedTouch) return // Se não é o nosso toque, ignorar
  }

  isDragging.value = false
  position.value = { x: 0, y: 0 }
  normalizedPosition.value = { x: 0, y: 0 }
  activeTouchId.value = null
  emit('move', { x: 0, y: 0 })
}

onMounted(() => {
  document.addEventListener('mousemove', handleMove)
  document.addEventListener('mouseup', handleEnd)
  document.addEventListener('touchmove', handleMove, { passive: false })
  document.addEventListener('touchend', handleEnd)
  document.addEventListener('touchcancel', handleEnd)
})

onUnmounted(() => {
  document.removeEventListener('mousemove', handleMove)
  document.removeEventListener('mouseup', handleEnd)
  document.removeEventListener('touchmove', handleMove)
  document.removeEventListener('touchend', handleEnd)
  document.removeEventListener('touchcancel', handleEnd)
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
