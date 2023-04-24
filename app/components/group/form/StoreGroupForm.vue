<!-- src/components/GroupForm.vue -->
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

const groupForm = useGroupForm();

const { form, title } = storeToRefs(groupForm);

const emit = defineEmits(["store"]);

const dialog = ref(false);

const open = async (id) => {
  groupForm.$reset();

  if (id) {
    await groupForm.show(id);
  }
  dialog.value = true;
};

const close = () => {
  dialog.value = false;
};

const handleSave = () => {
  $loading.start();
  groupForm
    .save()
    .then(() => {
      emit("store");
      dialog.value = false;
      $loading.finish();
    })
    .catch(() => {
      $loading.finish();
    });
};

defineExpose({ open });
</script>
