<template>
  <div>
    <template v-for="(product, index) in orderProducts">
      <div class="d-flex justify-space-between py-5">
        <div style="width: 60%">
          {{ product.product.name }}
        </div>
        <div style="width: 12%">
          <AppNumberField
            class="px-2"
            :model-value="product.quantity"
            variant="outlined"
            label="Quantidade"
            disabled
            bg-color="#ccc"
          />
        </div>
        <div style="width: 12%">
          <AppNumberField
            class="px-2"
            :model-value="product.price"
            variant="outlined"
            label="Preço"
            disabled
            bg-color="#ccc"
          />
        </div>
        <div style="width: 12%">
          <AppNumberField
            class="px-2"
            :model-value="product.total"
            variant="outlined"
            label="Preço"
            disabled
            bg-color="#ccc"
          />
        </div>
        <div style="width: 5%">
          <v-btn
            class="text-capitalize"
            icon
            variant="plain"
            @click="handleEdit(index)"
          >
            <v-icon icon="mdi:mdi-pencil" />
          </v-btn>
        </div>
        <div style="width: 5%">
          <v-btn
            class="text-capitalize"
            icon
            variant="plain"
            @click="handleRemove(index)"
          >
            <v-icon icon="mdi:mdi-delete" />
          </v-btn>
        </div>
      </div>
      <v-divider />
    </template>
  </div>
</template>

<script setup>
const emit = defineEmits(["edit"]);

const orderForm = useOrderFormStore();

const orderProducts = computed(() => orderForm.order.order_products);

const handleEdit = (index) => {
  emit("edit", index);
};

const handleRemove = (index) => {
  orderForm.removeOrderProduct(index);
};
</script>

<style lang="scss" scoped></style>