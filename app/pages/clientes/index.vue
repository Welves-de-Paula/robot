<template>
  <div>
    <v-row>
      <v-col cols="12" md="4">
        <AppSearchField
          v-model="query.search"
          @update:modelValue="getCustomers(1)"
        />
      </v-col>
      <v-col class="text-right d-md-block d-none" cols="8">
        <app-btn
          class="text-capitalize mr-2"
          bottom
          outlined
          to="/clientes/importar"
        >
          Importar
        </app-btn>
        <app-btn
          class="text-capitalize"
          bottom
          color="primary"
          @click="handleNewCustomer()"
        >
          Novo
        </app-btn>
      </v-col>
    </v-row>

    <v-row>
      <v-col>
        <AppPagination
          v-model="query.page"
          :data="customers"
          @update:modelValue="getCustomers()"
        >
          <v-list>
            <template v-for="(item, i) in customers.data">
              <v-list-item
                @click="handleEditCustomer(item.id)"
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
    <StoreCustomerForm @store="getCustomers()" ref="storeCustomerForm" />
    <!-- {{ customerStore.query }} -->
  </div>
</template>

<script setup>
definePageMeta({
  middleware: ["auth"],
  title: "Clientes",
});

const storeCustomerForm = ref(null);

const customerStore = useCustomerStore();

const query = computed(() => customerStore.query);

const customers = computed(() => customerStore.customers);

onMounted(() => {
  customerStore.$reset();

  getCustomers(1);
});

const getCustomers = async (page = null) => {
  if (page) {
    customerStore.query.page = page;
  }

  await customerStore.index();
};

const handleNewCustomer = () => {
  storeCustomerForm.value.open();
};

const handleEditCustomer = (id) => {
  storeCustomerForm.value.open(id);
};

// export default {

//     options: {},

//     loading: false,

//     searchParams: {
//       page: 1,
//       orderBy: "name",
//       orderDirection: "asc",
//     },

//     customers: {
//       data: [],
//     },
//   }),

//   created() {
//     this.select();
//   },

//   methods: {
//     select() {
//       this.$loading.start();
//       this.$http
//         .index("customer/customer", this.searchParams)
//         .then((response) => {
//           this.customers = response.customers;
//           this.$loading.finish();
//         })
//         .catch((error) => {
//           this.$loading.finish();
//         });
//     },

//     handleSearchInput(text) {
//       this.searchParams.text = text;
//       this.select();
//     },

//     handleNewCustomer(id) {
//       this.$refs.StoreCustomerForm.open(id);
//     },

//     pageChanged(page) {
//       this.searchParams.page = page;
//       this.select();
//     },
//   },
// };
//
</script>

<style></style>