<template>
  <div class="joysticks-page">
    <h1>Controle de Joysticks</h1>

    <div class="joysticks-wrapper">
      <Joystick label="Joystick Esquerdo" @move="handleLeftJoystick" />

      <Joystick label="Joystick Direito" @move="handleRightJoystick" />
    </div>

    <div class="control-panel">
      <div class="panel-section">
        <h3>🎮 Esquerdo</h3>
        <div class="values">
          <span>X: {{ leftJoystick.x.toFixed(3) }}</span>
          <span>Y: {{ leftJoystick.y.toFixed(3) }}</span>
        </div>
        <div class="direction">{{ getDirection(leftJoystick) }}</div>
      </div>

      <div class="panel-section">
        <h3>🎮 Direito</h3>
        <div class="values">
          <span>X: {{ rightJoystick.x.toFixed(3) }}</span>
          <span>Y: {{ rightJoystick.y.toFixed(3) }}</span>
        </div>
        <div class="direction">{{ getDirection(rightJoystick) }}</div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'

const leftJoystick = ref({ x: 0, y: 0 })
const rightJoystick = ref({ x: 0, y: 0 })

const handleLeftJoystick = (position: { x: number, y: number }) => {
  leftJoystick.value = position
}

const handleRightJoystick = (position: { x: number, y: number }) => {
  rightJoystick.value = position
}

const getDirection = (position: { x: number, y: number }): string => {
  const threshold = 0.3

  if (Math.abs(position.x) < threshold && Math.abs(position.y) < threshold) {
    return 'Centro'
  }

  const directions: string[] = []

  if (position.y < -threshold) directions.push('Cima')
  if (position.y > threshold) directions.push('Baixo')
  if (position.x < -threshold) directions.push('Esquerda')
  if (position.x > threshold) directions.push('Direita')

  return directions.join('-') || 'Centro'
}
</script>

<style scoped>
.joysticks-page {
  min-height: 100vh;
  background: linear-gradient(135deg, #1a1a2e 0%, #16213e 100%);
  padding: 2rem;
  color: white;
}

h1 {
  text-align: center;
  font-size: 2.5rem;
  margin-bottom: 3rem;
  color: #4a9eff;
  text-shadow: 0 0 10px rgba(74, 158, 255, 0.5);
}

.joysticks-wrapper {
  display: flex;
  justify-content: center;
  gap: 4rem;
  margin-bottom: 3rem;
  flex-wrap: wrap;
}

.control-panel {
  display: flex;
  justify-content: center;
  gap: 2rem;
  flex-wrap: wrap;
  max-width: 800px;
  margin: 0 auto;
}

.panel-section {
  background: rgba(255, 255, 255, 0.05);
  border-radius: 15px;
  padding: 1.5rem;
  min-width: 250px;
  backdrop-filter: blur(10px);
  border: 1px solid rgba(74, 158, 255, 0.3);
  box-shadow: 0 4px 15px rgba(0, 0, 0, 0.3);
}

.panel-section h3 {
  margin-top: 0;
  color: #4a9eff;
  font-size: 1.5rem;
  text-align: center;
  margin-bottom: 1rem;
}

.values {
  display: flex;
  flex-direction: column;
  gap: 0.5rem;
  font-family: 'Courier New', monospace;
  font-size: 1.1rem;
  margin-bottom: 1rem;
}

.values span {
  background: rgba(0, 0, 0, 0.3);
  padding: 0.5rem;
  border-radius: 5px;
  border-left: 3px solid #4a9eff;
}

.direction {
  text-align: center;
  font-size: 1.2rem;
  font-weight: bold;
  color: #5aafff;
  padding: 0.75rem;
  background: rgba(74, 158, 255, 0.1);
  border-radius: 8px;
  min-height: 2.5rem;
  display: flex;
  align-items: center;
  justify-content: center;
}

@media (max-width: 768px) {
  h1 {
    font-size: 2rem;
  }

  .joysticks-wrapper {
    gap: 2rem;
  }

  .control-panel {
    flex-direction: column;
    align-items: center;
  }

  .panel-section {
    width: 100%;
    max-width: 300px;
  }
}

/* Otimização para dispositivos móveis em modo horizontal */
@media (max-width: 926px) and (orientation: landscape) {
  .joysticks-page {
    padding: 1rem;
    min-height: 100vh;
    display: flex;
    flex-direction: column;
  }

  h1 {
    font-size: 1.5rem;
    margin-bottom: 1rem;
  }

  .joysticks-wrapper {
    flex: 1;
    display: flex;
    justify-content: space-around;
    align-items: center;
    gap: 2rem;
    margin-bottom: 1rem;
  }

  .control-panel {
    flex-direction: row;
    gap: 1rem;
    max-width: 100%;
  }

  .panel-section {
    min-width: auto;
    flex: 1;
    padding: 0.75rem;
  }

  .panel-section h3 {
    font-size: 1rem;
    margin-bottom: 0.5rem;
  }

  .values {
    font-size: 0.85rem;
    gap: 0.25rem;
  }

  .values span {
    padding: 0.25rem 0.5rem;
  }

  .direction {
    font-size: 0.9rem;
    padding: 0.5rem;
    min-height: 2rem;
  }
}

/* Telas muito pequenas em horizontal (smartphones) */
@media (max-height: 500px) and (orientation: landscape) {
  .joysticks-page {
    padding: 0.5rem;
  }

  h1 {
    font-size: 1.2rem;
    margin-bottom: 0.5rem;
  }

  .joysticks-wrapper {
    margin-bottom: 0.5rem;
  }

  .control-panel {
    gap: 0.5rem;
  }

  .panel-section {
    padding: 0.5rem;
  }

  .panel-section h3 {
    font-size: 0.9rem;
    margin-bottom: 0.25rem;
  }

  .values {
    font-size: 0.75rem;
  }

  .direction {
    font-size: 0.8rem;
    padding: 0.25rem;
  }
}
</style>
