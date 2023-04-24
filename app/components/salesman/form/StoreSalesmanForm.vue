<template>
  <v-row justify="center">
    <v-dialog v-model="dialog" persistent width="700">
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
              <AppStatusSwitch v-model="form.status" />
            </v-col>
            <v-col>
              <v-autocomplete
                label="Regiões"
                :items="regions.data"
                v-model="form.regions"
                item-title="name"
                :item-value="getRegionId"
                multiple
              />
            </v-col>
          </v-row>
        </v-card-text>
        <v-card-actions>
          <v-btn variant="text" @click="dialog = false"> Cancelar </v-btn>
          <AppStoreBtn @click="handleSave()"> Salvar </AppStoreBtn>
        </v-card-actions>
      </v-card>
    </v-dialog>
  </v-row>
</template>


<script setup>
const emit = defineEmits(["store"]);
const salesmanForm = useSalesmanStoreForm();

const { form, title } = storeToRefs(salesmanForm);

const { $loading } = useNuxtApp();

const dialog = ref(false);

const open = (id) => {
  getRegions();

  dialog.value = true;

  salesmanForm.$reset();

  if (id) {
    $loading.start();
    salesmanForm
      .show(id)
      .then((response) => {
        $loading.finish();
      })
      .catch((error) => {
        $loading.finish();
      });
  }
};

const regionStore = useRegionStore();

const regions = computed(() => regionStore.regions);

const getRegions = async () => {
  $loading.start();
  await regionStore
    .index()
    .then((response) => {
      $loading.finish();
    })
    .catch((error) => {
      $loading.finish();
    });
};

const getRegionId = (region) => {
  return { id: region.id };
};

const handleSave = () => {
  try {
    $loading.start();
    salesmanForm
      .save()
      .then(() => {
        dialog.value = false;
        emit("store");
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
