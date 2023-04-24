<template>
  <div>
    <v-row class="d-flex align-center">
      <v-col cols="5">
        <SearchProduct
          v-model="form.product"
          label="Produto"
          @input="setProduct()"
        />
      </v-col>
      <v-col cols="2">
        <AppNumberField v-model="form.quantity" label="Quantidade" />
      </v-col>
      <v-col cols="2">
        <AppNumberField
          v-model="form.price"
          label="Preço"
          currencyDisplay="symbol"
        />
      </v-col>
      <v-col cols="2">
        <AppNumberField
          :model-value="net_total"
          variant="outlined"
          label="Total"
          disabled
          bg-color="#ccc"
          currencyDisplay="symbol"
        />
      </v-col>

      <v-btn
        class="ml-auto mr-auto"
        icon="mdi-plus"
        color="primary"
        size="small"
        @click="add()"
      />
    </v-row>
  </div>
</template>

<script setup>
const orderForm = useOrderFormStore();

const { $format } = useNuxtApp();

const { form } = storeToRefs(orderForm);

const net_total = computed(() => {
  form.value.total = $format.decimal(
    parseFloat(form.value.price) * parseFloat(form.value.quantity)
  );

  return form.value.total;
});

const add = () => {
  orderForm.storeOrderProduct();
};

const setProduct = () => {
  form.value.price =
    form.value.product && form.value.product.product_variant
      ? form.value.product.product_variant.price
      : 0;

  form.value.price = $format.decimal(form.value.price, 2);
};
</script>

<style>
</style>