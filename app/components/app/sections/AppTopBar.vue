<template>
  <v-app-bar :elevation="0">
    <v-app-bar-nav-icon @click.stop="handleChangeDrawer()" />

    <v-toolbar-title>
      <h3 class="d-inline">{{ $route.meta.title }}</h3>
      <v-chip class="ml-2" v-if="showDevelopmentWarningChip" color="error">
        Ambiente de teste
      </v-chip>
    </v-toolbar-title>
    <br />
  </v-app-bar>
</template>

<script>
export default {
  computed: {
    showDevelopmentWarningChip() {
      return process.env.NODE_ENV === "development";
    },
  },
};
</script>

<style>
</style>