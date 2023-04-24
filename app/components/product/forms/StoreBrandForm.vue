<!-- src/components/BrandForm.vue -->
<template>
  <v-dialog v-model="dialog" max-width="600px" persistent>
    <v-card :title="title">
      <v-card-text>
        <v-container>
          <v-row>
            <v-col cols="12">
              <AppTextField v-model="form.name" label="Nome" />
            </v-col>

            <v-col cols="12">
              <AppStatusSwitch v-model="form.status" />
            </v-col>
          </v-row>
        </v-container>
      </v-card-text>

      <v-card-actions>
        <v-btn variant="text" @click="dialog = false"> Cancelar </v-btn>
        <AppStoreBtn @click="handleSave()"> Salvar </AppStoreBtn>
      </v-card-actions>
    </v-card>
  </v-dialog>
</template>

<script setup>

const { $loading } = useNuxtApp();

const brandStore = useBrandStore();

const emit = defineEmits(["store"]);

const dialog = ref(false);

const form = computed(() => brandStore.brandForm);

const title = computed(() => (form.id) ? 'Editar Marca' : 'Nova Marca');

const open = async (id = null) => {
  
  brandStore.resetForm();

  if (id) {
    await brandStore.loadForm(id);
  }

  dialog.value = true;
};

const close = () => {
  dialog.value = false;
};

const handleSave = async () => {
  
  $loading.start();

  try {

    await brandStore.storeForm()

    close();

    emit("store");
  
  } catch (error) {
    console.log(error);
  }

  $loading.stop();
 
};

defineExpose({ open });
</script>
