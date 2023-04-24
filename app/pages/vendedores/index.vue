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
          @click="handleNewSalesman()"
        >
          Novo
        </app-btn>
      </v-col>
    </v-row>

    <v-row>
      <v-col>
        <AppPagination
          v-model="query.page"
          :data="salesmen"
          @update:modelValue="select()"
        >
          <v-list>
            <template v-for="(item, i) in salesmen.data" density="compact">
              <v-list-item
                @click="handleEditSalesman(item.id)"
                :value="item.id"
              >
                <template v-slot:prepend>
                  <small class="mr-2">{{ item.code }}# </small>
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
    <StoreSalesmanForm @store="select()" ref="storeSalesmanForm" />
  </div>
</template>

<script setup>
definePageMeta({
  middleware: ["auth"],
  title: "Vendedores",
});
const { $loading } = useNuxtApp();

const storeSalesmanForm = ref(null);

const salesmanStore = useSalesmanStore();

const query = computed(() => salesmanStore.query);

const salesmen = computed(() => salesmanStore.salesmen);

onMounted(() => {
  salesmanStore.$reset();

  select(1);
});

const select = async (page = null) => {
  $loading.start();
  if (page) {
    salesmanStore.query.page = page;
  }

  await salesmanStore
    .index()
    .then(() => {
      $loading.finish();
    })
    .catch(() => {
      $loading.finish();
    });
};

const handleNewSalesman = () => {
  storeSalesmanForm.value.open();
};

const handleEditSalesman = (id) => {
  storeSalesmanForm.value.open(id);
};
</script>

<style></style>