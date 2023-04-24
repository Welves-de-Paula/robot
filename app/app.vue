<template>
  <v-app>
    <NuxtLayout>
      <NuxtPage />
    </NuxtLayout>
    <AppLoading />
  </v-app>
</template>
<script setup>
useHead({
  title: "ERP - Tintas Antares",
});
</script>

<style>
@import url("https://fonts.googleapis.com/css2?family=Montserrat:ital,wght@0,200;0,500;1,400&display=swap");

.v-application {
  font-family: "Montserrat", sans-serif;
}

/* apply font-family: "Montserrat", sans-serif; to v-card class and v-card-text in a sigle */
.v-card,
.v-card-text {
  font-family: "Montserrat", sans-serif;
}
</style>
