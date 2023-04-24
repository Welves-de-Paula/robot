<template>
  <app-mask-field
    dense
    @input="handleInput($event)"
    :loading="loading"
    mask="##.###.###/####-##"
  />
</template>

<script>
import axios from "axios";

export default {
  props: {
    isInsert: {
      type: Boolean,
      default: false,
    },
  },

  data: () => ({
    loading: false,
  }),
  methods: {
    handleInput({ _rawValue }) {
      var cnpj = _rawValue;

      cnpj = cnpj.replace(/[^\d]+/g, "");
      if (cnpj.length == 14 && this.isInsert) {
        console.log("cnpj", cnpj);
        this.searchCnpj(cnpj);
      }
    },

    async searchCnpj(cnpj) {
      this.loading = true;

      await axios
        .get("https://brasilapi.com.br/api/cnpj/v1/" + cnpj)
        .then((response) => {
          this.$emit("company", {
            emails: [{ contact: null, marker: null }],
            sites: [{ contact: null, marker: null }],
            person_type: "juridica",
            name: response.data.razao_social,
            alias: response.data.nome_fantasia,
            cnpj: response.data.cnpj,
            birth_date: response.data.data_inicio_atividade,
            main_address: {
              address: response.data.logradouro,
              city: response.data.municipio,
              city_code: response.data.codigio_municipio,
              complement: response.data.complemento,
              country: response.data.pais,
              country_code: response.data.codigo_pais,
              district: response.data.bairro,
              number: response.data.numero,
              postal_code: response.data.cep,
              state: response.data.uf,
              type: "mainAddress",
            },
            phones: [
              { contact: response.data.ddd_telefone_1, marker: "comercial" },
              { contact: response.data.ddd_fax, marker: "comercial" },
            ],
          });
        });

      this.loading = false;
    },
  },
};
</script>
