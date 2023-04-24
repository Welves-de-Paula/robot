<template>
  <v-dialog v-model="dialog" persistent width="1024" scrollable>
    <v-card :title="title">
      <v-card-text>
        <h3 class="mb-2">Informações</h3>
        <v-row>
          <v-col cols="7" md="6">
            <v-radio-group
              class="mt-0"
              hide-details="auto"
              v-model="form.person_type"
              inline
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
              <app-text-field v-model="form.rg" label="RG" />
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
            <!-- <RegionSearch v-model="form.region" /> -->
          </v-col>
        </v-row>

        <v-row>
          <v-col cols="12" md="5">
            <app-text-field
              v-model="form.main_address.address"
              label="Endereço"
            />
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
            <app-text-field
              v-model="form.main_address.district"
              label="Bairro"
            />
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
            <AppStatusSwitch v-model="form.status" />
          </v-col>
        </v-row>
      </v-card-text>
      <v-card-actions>
        <v-btn variant="text" @click="dialog = false"> Cancelar </v-btn>
        <v-btn
          variant="elevated"
          theme="dark"
          color="success"
          @click="handleSave()"
        >
          Salvar
        </v-btn>
      </v-card-actions>
    </v-card>
  </v-dialog>
</template>
 
<script setup>
import { defineEmits } from "vue";

const dialog = ref(false);

const loading = useNuxtApp().$loading;

const statusText = {
  1: "Este cadastro está ativo",
  0: "Este cadastro está inativo",
};

const customerForm = useCustomerStoreForm();
const { form, title, IsJuridic, isInsert, alias_label, name_label } =
  storeToRefs(customerForm);

const emit = defineEmits(["store"]);

const open = (id) => {
  loading.start();
  dialog.value = true;

  customerForm.reset();

  if (id) {
    customerForm.show(id);
  }

  setDefaults();
  loading.finish();
};

const setDefaults = () => {
  form.value.phones.push({
    contact: null,
    marker: "comercial",
  });

  form.value.emails.push({
    contact: null,
  });

  if (form.value.sites.length == 0) {
    form.value.sites.push({
      contact: null,
    });
  }
};

const setCompanyData = (company) => {
  form.value = {
    ...form.value,
    ...company,
  };
};

const handleCepInput = (address) => {
  form.value.main_address.city = address.city;
  form.value.main_address.state = address.state;
  form.value.main_address.city_code = address.city_code;
};

const handleSave = () => {
  customerForm.save().then(() => {
    emit("store");
    dialog.value = false;
  });
};

defineExpose({ open });
</script>

<style scoped>
</style>