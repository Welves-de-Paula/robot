<template>
  <app-form-dialog
    v-model="dialog"
    :title="title"
    width="950px"
    @close="dialog = false"
    @confirm="handleSave()"
  >
    <h3 class="mb-2">Informações</h3>
    <v-row>
      <v-col cols="7" md="6">
        <v-radio-group
          class="mt-0"
          hide-details="auto"
          v-model="form.person_type"
          row
        >
          <v-radio label="Pessoa Física" value="fisica"> </v-radio>
          <v-radio label="Pessoa Jurídica" value="juridica"> </v-radio>
        </v-radio-group>
      </v-col>
      <v-col offset-md="4" md="2">
        <app-text-field
          outlined
          disabled
          label="Código"
          readonly
          :value="form.code"
          reverse
        />
      </v-col>
    </v-row>
    <v-row>
      <v-col cols="12" md="6">
        <app-text-field v-model="form.name" :label="name_label" />
      </v-col>
      <v-col cols="12" md="6">
        <app-text-field v-model="form.alias" :label="alias_label" />
      </v-col>
    </v-row>
    <v-row>
      <template v-if="!IsJuridic">
        <v-col>
          <app-cpf-field v-model="form.cpf" label="CPF" />
        </v-col>
        <v-col>
          <app-rg-field v-model="form.rg" label="RG" />
        </v-col>
      </template>
      <template v-if="IsJuridic">
        <v-col>
          <app-cnpj-field
            @company="setCompanyData($event)"
            v-model="form.cnpj"
            :isInsert="isInsert"
            label="CNPJ"
          />
        </v-col>
        <v-col>
          <app-text-field
            :disabled="form.indicador_ie == 1"
            v-model="form.inscricao_estadual"
            label="Inscrição Estadual"
          />
        </v-col>
      </template>
    </v-row>
    <h3 class="mt-10 mb-6">Endereços</h3>
    <v-row>
      <v-col md="4">
        <app-cep-field
          v-model="form.main_address.postal_code"
          @address="handleCepInput($event)"
        />
      </v-col>
      <v-col md="4">
        <RegionSearch v-model="form.region" />
      </v-col>
    </v-row>

    <v-row>
      <v-col cols="12" md="5">
        <app-text-field v-model="form.main_address.address" label="Endereço" />
      </v-col>
      <v-col md="2">
        <app-text-field v-model="form.main_address.number" label="Número" />
      </v-col>
      <v-col md="5">
        <app-text-field
          v-model="form.main_address.complement"
          label="Complemento"
        />
      </v-col>
    </v-row>
    <v-row>
      <v-col cols="12" md="5">
        <app-text-field v-model="form.main_address.district" label="Bairro" />
      </v-col>
      <v-col md="5">
        <app-text-field
          v-model="form.main_address.city"
          label="Cidade"
          readonly
        />
      </v-col>
      <v-col md="2">
        <app-text-field
          v-model="form.main_address.state"
          label="Estado"
          readonly
        />
      </v-col>
    </v-row>

    <h3 class="mt-10 mb-4">Telefones</h3>
    <v-row>
      <v-col cols="6">
        <app-text-field v-model="form.phone_wa" label="WhatsApp" />
      </v-col>
    </v-row>
    <v-row>
      <v-col>
        <PhoneContactsForm v-model="form.phones" />
      </v-col>
    </v-row>
    <h3 class="mt-10 mb-4">Emails e sites</h3>
    <v-row>
      <v-col>
        <EmailContactsForm v-model="form.emails" />
      </v-col>
    </v-row>
    <v-row>
      <v-col>
        <SiteContactsForm v-model="form.sites" />
      </v-col>
    </v-row>

    <v-row>
      <v-col>
        <app-status-switch v-model="form.status" />
      </v-col>
    </v-row>
  </app-form-dialog>
</template>

<script>
import PhoneContactsForm from "@/components/app/forms/PhoneContactsForm.vue";
import EmailContactsForm from "@/components/app/forms/EmailContactsForm.vue";
import SiteContactsForm from "@/components/app/forms/SiteContactsForm.vue";
import RegionSearch from "@/components/region/ui/RegionSearch.vue";

export default {
  components: {
    PhoneContactsForm,
    EmailContactsForm,
    SiteContactsForm,
    RegionSearch,
  },
  data() {
    return {
      dialog: false,
      statusText: {
        1: "Este cadastro está ativo",
        0: "Este cadastro está inativo",
      },
      nameRules: [(v) => !!v || "Name is required"],

      valid: false,
      form: {},
      baseForm: {
        code: null,
        person_type: "fisica",
        name: null,
        alias: null,
        cpf: null,
        cnpj: null,
        rg: null,
        status: 1,
        inscricao_estadual: null,

        main_address: {
          address: null,
          number: null,
          complement: null,
          district: null,
          postal_code: null,
          city_district: null,
          city: null,
          state: null,
          country: null,
          type: "mainAddress",
          region: null,
          region_id: null,
        },
        phones: [],
        emails: [],
        sites: [],
      },
    };
  },
  computed: {
    IsJuridic() {
      return this.form.person_type == "juridica";
    },
    alias_label() {
      if (this.IsJuridic) {
        return "Nome Fantasia";
      } else {
        return "Apelido";
      }
    },

    name_label() {
      if (this.IsJuridic) {
        return "Razão Social";
      } else {
        return "Nome";
      }
    },
    isInsert() {
      return !this.form.id;
    },
    title() {
      if (this.isInsert) {
        return "Cadastrar Cliente";
      } else {
        return "Editar Cliente";
      }
    },
  },
  created() {
    this.reset();
  },

  methods: {
    open(id) {
      this.reset();
      if (id) {
        this.show(id);
      }

      this.dialog = true;
    },

    reset() {
      this.form = JSON.parse(JSON.stringify(this.baseForm));
    },
    async show(id) {
      this.$loading.start();
      await this.$http
        .show("customer/customer", id)
        .then((response) => {
          this.form = JSON.parse(JSON.stringify(response.customer));

          this.$loading.finish();
        })
        .catch((error) => {
          this.$loading.finish();
        });
    },
    setDefaults() {
      this.form.phones.push({
        contact: null,
      });

      this.form.emails.push({
        contact: null,
      });

      if (this.form.sites.length == 0) {
        this.form.sites.push({
          contact: null,
        });
      }
    },

    handleSave() {
      if (this.isInsert) {
        this.store();
      }
      if (!this.isInsert) {
        this.update();
      }
    },

    store() {
      this.$loading.start();
      this.$http
        .store("customer/customer", this.form)
        .then((response) => {
          this.$loading.finish();
          this.$emit("store");
          this.dialog = false;
        })
        .catch((error) => {
          this.$loading.finish();
        });
    },

    update() {
      this.$loading.start();
      this.$http
        .update("customer/customer", this.form.id, this.form)
        .then((response) => {
          this.$loading.finish();
          this.$emit("store");
          this.dialog = false;
        })
        .catch((error) => {
          this.$loading.finish();
        });
    },
    setCompanyData(company) {
      this.form = {
        ...this.form,
        ...company,
      };
    },
    handleCepInput(address) {
      this.form.main_address.city = address.city;
      this.form.main_address.state = address.state;
      this.form.main_address.city_code = address.city_code;
    },
  },
};
</script>

<style scoped>
</style>