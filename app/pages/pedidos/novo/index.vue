<template>
  <div>
    <div
      class="mb-10 pa-5"
      style="border: 1px solid #ccc; padding-bottom: 15px"
    >
      <v-row>
        <v-col>
          <SearchCustomer label="Cliente" />
        </v-col>
        <v-col>
          <SearchSalesman label="Vendedor" />
        </v-col>
      </v-row>
    </div>

    <div
      class="mb-10 pa-5"
      style="border: 1px solid #ccc; padding-bottom: 15px"
    >
      <h3 class="pa-3">Produtos</h3>

      <OrderProductForm ref="orderProductForm" />

      <v-divider class="my-4" />
      <v-row>
        <v-col>
          <OrderProductsList @edit="handleEditOrderProduct($event)" />
          <!-- <v-btn color="primary" @click="handleAddProduct()">
          Adicionar produto
        </v-btn> -->
        </v-col>
      </v-row>
    </div>
    <v-row>
      <v-col>
        total
        {{ total }}
      </v-col>
    </v-row>
  </div>
</template>

<script setup>
definePageMeta({
  middleware: ["auth"],
  title: "Novo Venda",
});
const orderProductForm = ref(null);

const form = ref(null);

const orderForm = useOrderFormStore();

const handleAddProduct = () => {
  orderProductForm.value.add();
};

const handleEditOrderProduct = (index) => {
  orderProductForm.value.edit(index);
};

const total = computed(() => {
  return orderForm.total;
});
</script>

<style lang="scss" scoped>
</style>