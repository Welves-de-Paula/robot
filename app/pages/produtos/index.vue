<template>
  <div>
    <v-row>
      <v-col cols="12" md="4">
        <AppSearchField v-model="query.search" @update:modelValue="select(1)" />
      </v-col>
      <v-col class="text-right d-md-block d-none" cols="8">
        <app-btn
          class="text-capitalize"
          bottom
          color="primary"
          @click="handleNewProduct()"
        >
          Novo
        </app-btn>
      </v-col>
    </v-row>

    <v-row>
      <v-col>
        <AppPagination
          v-model="query.page"
          :data="products"
          @update:modelValue="select()"
        >
          <v-list>
            <template v-for="(item, i) in products.data">
              <v-list-item @click="handleEditProduct(item.id)" :value="item.id">
                <template v-slot:prepend>
                  <!-- <small class="mr-2">{{ item.code }}# </small> -->
                </template>
                <v-list-item-title>
                  {{ item.name }}
                </v-list-item-title>
                <template v-slot:append>
                  <AppStatusChip :value="item.status" />
                </template>
              </v-list-item>
              <v-divider />
            </template>
          </v-list>
        </AppPagination>
      </v-col>
    </v-row>
    <StoreProductForm @store="select()" ref="storeProductForm" />
  </div>
</template>

<script setup>
definePageMeta({
  middleware: ["auth"],
  title: "Produtos",
});
const { $loading } = useNuxtApp();

const storeProductForm = ref(null);

const productStore = useProductStore();

const query = computed(() => productStore.query);

const products = computed(() => productStore.products);

onMounted(() => {
  productStore.$reset();

  select(1);
});

const select = async (page = null) => {
  $loading.start();
  if (page) {
    productStore.query.page = page;
  }

  await productStore
    .index()
    .then((response) => {
      $loading.finish();
    })
    .catch((error) => {
      $loading.finish();
    });
};

const handleNewProduct = () => {
  storeProductForm.value.open();
};

const handleEditProduct = (id) => {
  storeProductForm.value.open(id);
};
</script>

<style></style>