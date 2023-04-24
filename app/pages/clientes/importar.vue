<template>
  <div>
    <v-row>
      <v-col>
        <AppFileField
          accept=".xlsx"
          url="customer/import"
          @input="onSuccess($event)"
        />
      </v-col>
    </v-row>

    <v-row>
      <v-col>
        <v-data-table :headers="headers" :items="customers"> </v-data-table>
      </v-col>
    </v-row>
  </div>
</template>

<script setup>
definePageMeta({
  middleware: ["auth"],
  title: "Importar Clientes",
});

const headers = ref([
  { value: "cod", text: "external_id" },
  { value: "name", text: "Nome/Razão Social" },
  { value: "alias", text: "Apelido/Nome Fantasia" },
  { value: "city", text: "Cidade" },
  { value: "phone", text: "Telefone" },
  { value: "cellphone", text: "Celular" },
  { value: "email", text: "E-mail" },
  { value: "status", text: "Status" },
]);

const customers = ref([]);

const loading = useNuxtApp().$loading;

const hasCustomers = computed(() => customers.value.length > 0);

const onSuccess = (response) => {
  customers.value = JSON.stringify(JSON.parse(response));
};
</script>

<style>
</style>