<template>
  <v-layout>
    <AppNavigationDrawer />
    <AppTopBar />
    <v-main>
      <v-container class="pa-md-8" fluid>
        <slot />
      </v-container>
    </v-main>
  </v-layout>
</template>
<script setup>
</script>

 