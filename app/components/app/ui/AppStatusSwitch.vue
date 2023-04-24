

<template>
  <v-switch
    v-model="switchValue"
    hide-details="auto"
    color="primary"
    :label="label"
  />
</template>

<script setup>
const emit = defineEmits(["update:modelValue"]);

const props = defineProps({
  modelValue: {},
});

const switchValue = computed({
  get() {
    return props.modelValue;
  },

  set(newValue) {
    emit("update:modelValue", newValue);
  },
});

const label = computed(() => {
  return switchValue.value ? "Cadastro Ativo" : "Cadastro Inativo";
});
</script>
