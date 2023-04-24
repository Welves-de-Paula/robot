<template>
  <v-chip :size="props.size" :color="color">{{ label }}</v-chip>
</template>

<script setup>
const props = defineProps({
  value: {
    type: Boolean,
    default: true,
  },
  size: {
    type: String,
    default: "small",
  },
});

const color = computed(() => {
  return props.value == true ? "success" : "error";
});

const label = computed(() => {
  return props.value == true ? "Ativo" : "Inativo";
});
</script>

<style>
</style>