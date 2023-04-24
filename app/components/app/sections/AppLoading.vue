<template>
  <v-overlay
    :model-value="isLoading"
    class="align-center justify-center"
    id="loading"
  >
    <v-progress-circular color="primary" indeterminate size="64" />
  </v-overlay>
</template>

<script setup>
const appStore = useAppStore();

const isLoading = computed(() => appStore.isLoading);
</script>


<style scoped lang='sass'>
#loading
  z-index: 999999
</style>