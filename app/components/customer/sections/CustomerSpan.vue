<template>
  <span v-bind="$attrs">{{ text }}</span>
</template>

<script>
export default {
  props: {
    customer : {}
  },

  computed : {
    text () {
      if (this.customer) {
        return this.customer.name;
      }

      return 'Consumidor Final';
    }
  }
};
</script>

<style></style>
