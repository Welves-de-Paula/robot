<template>
  <app-text-field
    v-model="searchTerm"
    variant="outlined"
    label="Pesquisar"
    prepend-inner-icon="mdi-magnify"
  />
</template>

<script setup>
import { debounce } from "lodash";

const searchTerm = ref(null);

const emit = defineEmits(["update:modelValue"]);

const debouncedSearch = debounce((value) => {
  emit("update:modelValue", value);
}, 300);

watch(searchTerm, (value) => {
  debouncedSearch(value);
});
</script>