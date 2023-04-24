<template>
  <app-text-field
    v-model="codeValue"
    v-bind="$attrs"
    label="Código"
    :append-outer-icon="icon"
    :readonly="autoMode"
    @click:append-outer="autoMode = !autoMode"
  />
</template>

<script>
export default {
  props: {
    value : {},
    clearOnAutoCode : {}
  },

  data: () => ({
    codeValue : null,
    autoMode : true
  }),

  computed : {
    icon() {
      return (this.autoMode) ? 'mdi-pencil' : 'mdi-flash-auto'
    },
  },

  watch : {
    value(value) {
      this.setCodeValue(value)
    },

    codeValue(value) {
      this.emitInput(value)
    },

    autoMode() {
      if (this.clearOnAutoCode && this.autoMode) {
        this.codeValue = null
      }
    }
  },

  created() {
    this.setCodeValue(this.value)
  },

  methods : {
    setCodeValue(value) {
      this.codeValue = value
    },

    emitInput(value) {
      this.$emit('input', value)
    }
  }
};
</script>
