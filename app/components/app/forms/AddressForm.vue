<template>
  <div>
    <v-row align-end>
      <v-col class="pb-0" md="4">
        <app-cep-field
          v-model="address.postal_code"
          @address="handleCepInput($event)"
        />
      </v-col>

      <v-btn
        v-if="address.type != 'mainAddress'"
        class="ml-auto mt-auto text-capitalize"
        primary
        x-small
        text
        @click="handleRemove()"
      >
        Remover
        <v-icon x-small>mdi-close</v-icon>
      </v-btn>
    </v-row>

    <v-row>
      <v-col cols="12" md="5">
        <app-text-field v-model="address.address" label="Endereço" />
      </v-col>
      <v-col md="2">
        <app-text-field v-model="address.number" label="Número" />
      </v-col>
      <v-col md="5">
        <app-text-field v-model="address.complement" label="Complemento" />
      </v-col>
    </v-row>
    <v-row>
      <v-col cols="12" md="5">
        <app-text-field v-model="address.district" label="Bairro" />
      </v-col>
      <v-col md="5">
        <app-text-field v-model="address.city" label="Cidade" readonly />
      </v-col>
      <v-col md="2">
        <app-text-field v-model="address.state" label="Estado" readonly />
      </v-col>
    </v-row>
  </div>
</template>
<script setup>
import { ref, watch } from "vue";

const props = defineProps({
  value: {},
});

const address = ref({
  address: null,
  addressable_id: null,
  addressable_type: null,
  city: null,
  city_code: null,
  complement: null,
  country: null,
  country_code: null,
  created_at: null,
  district: null,
  id: null,
  number: null,
  postal_code: null,
  state: null,
  state_code: null,
  status: null,
  type: null,
});

const setAddress = (value) => {
  address.value = value;
};

const handleCepInput = (address) => {
  address.city = address.localidade;
  address.address = address.logradouro;
  address.district = address.bairro;
  address.state = address.uf;
  address.complement = address.complemento;
  address.city_code = address.ibge;
};

const handleRemove = () => {
  address.value.status = 0;
};

watch(
  () => props.value,
  (val) => {
    setAddress(val);
  }
);

watch(
  address,
  (val) => {
    emit("update:value", val);
  },
  { deep: true }
);

setAddress(props.value);
</script>

<style>
</style>
