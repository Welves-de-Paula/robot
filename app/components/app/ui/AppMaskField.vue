
<template>
  <v-text-field v-maska:[options] v-model="dataInput" @change="handleInput" />
</template>


<script setup>
import { vMaska } from "maska";

const emit = defineEmits(["input"]);

const attrs = useAttrs();

const options = reactive({
  mask: attrs.mask,
  eager: true,
});

const dataInput = ref("");

const handleInput = () => {
  emit("input", dataInput);
};
</script>
 


