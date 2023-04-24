<template>
  <v-select
    outlined
    v-bind="$attrs"
    hide-details="auto"
    v-on="$listeners"
    :items="options"
  />
</template>

<script>
export default {
  data() {
    return {
      options: [
        { value: 1, text: "1 - Contribuinte ICMS" },
        { value: 2, text: "2 - Contribuinte isento" },
        { value: 9, text: "9 - Não Contribuinte" },
      ],
    };
  },
};
</script>

<style>
</style>