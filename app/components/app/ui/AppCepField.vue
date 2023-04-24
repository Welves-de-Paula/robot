<template>
  <app-mask-field
    hide-details="auto"
    dense
    append-outer-icon="mdi-magnify"
    label="CEP"
    @input="handleInput($event)"
    :loading="loading"
    mask="#####-###"
  />
</template>

<script>
import axios from "axios";

export default {
  data: () => ({
    loading: false,
  }),

  methods: {
    handleInput({ _rawValue }) {
      var cep = _rawValue;

      cep = cep.replace(/[^\d]+/g, "");

      if (cep.length == 8) {
        this.searchCep(cep);
      }
    },

    async searchCep(cep) {
      this.loading = true;

      await axios
        .get(`https://viacep.com.br/ws/${cep}/json/`)
        .then((response) => {
          this.$emit("address", {
            city: response.data.localidade,
            state: response.data.uf,
            city_code: response.data.ibge,
          });
        });

      this.loading = false;
    },
  },
};
</script>