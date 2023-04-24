<template>
  <v-container>
    <h3>Bem vindo ao sistema de gestão de clientes</h3>
  </v-container>
</template>

<script setup>

definePageMeta({
  middleware: ["auth"],
  title: "Dashboard",
})

</script>


