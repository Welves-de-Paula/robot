<template>
  <v-row justify="center">
    <v-dialog v-model="dialog" persistent width="800">
      <v-card :title="title">
        <v-card-text>
          <h3 class="mb-2">Informações</h3>

          <v-row>
            <v-col>
              <AppTextField v-model="form.name" label="Nome" />
            </v-col>
          </v-row>

          <v-row>
            <v-col>
              <AppTextField v-model="form.email" label="Email" />
            </v-col>

            <v-col>
              <AppTextField v-model="form.whatsapp" label="WhatsApp" />
            </v-col>
          </v-row>

          <v-row>
            <v-col>
              <AppPasswordField v-model="form.password" label="Senha" />
            </v-col>
            <v-col>
              <AppPasswordField
                v-model="form.password_confirmation"
                label="Senha"
              />
            </v-col>
          </v-row>
          <v-row>
            <v-col>
              <AppStatusSwitch v-model="form.status" />
            </v-col>
            <v-col>
              <v-autocomplete
                label="Vendedor"
                :items="salesmen.data"
                v-model="form.salesman_id"
                clearable
                item-title="name"
                item-value="id"
              />
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
  </v-row>
</template>


<script setup>
const userForm = useUserStoreForm();
const emit = defineEmits(["store"]);

const { form, title } = storeToRefs(userForm);

const { $loading } = useNuxtApp();

const dialog = ref(false);

const open = (id) => {
  $loading.start();

  getSalesmen();

  dialog.value = true;

  userForm.$reset();

  if (id) {
    userForm.show(id);
  }

  $loading.finish();
};

const salesmanStore = useSalesmanStore();

const salesmen = computed(() => salesmanStore.salesmen);

const getSalesmen = async () => {
  await salesmanStore.index();
};

const getSalesmanId = (salesman) => {
  return { id: salesman.id };
};

const handleSave = () => {
  try {
    $loading.start();
    userForm
      .save()
      .then(() => {
        emit("store");
        dialog.value = false;
      })
      .catch(() => {
        $loading.finish();
      });
  } catch (error) {
    $loading.finish();
  }
};

defineExpose({ open });
</script>
