<template>
  <div>
    <v-row>
      <v-col cols="12" md="4">
        <AppSearchField v-model="query.search" @update:modelValue="select(1)" />
      </v-col>
      <v-col class="text-right d-md-block d-none" cols="8">
        <app-btn
          class="text-capitalize"
          bottom
          color="primary"
          @click="handleNewUser()"
        >
          Novo
        </app-btn>
      </v-col>
    </v-row>

    <v-row>
      <v-col>
        <AppPagination
          v-model="query.page"
          :data="users"
          @update:modelValue="select()"
        >
          <v-list>
            <template v-for="(item, i) in users.data">
              <v-list-item @click="handleEditUser(item.id)" :value="item.id">
                <template v-slot:prepend>
                  <!-- <small class="mr-2">{{ item.code }}# </small> -->
                </template>
                <v-list-item-title>
                  {{ item.name }}
                </v-list-item-title>
                <template v-slot:append>
                  <AppStatusChip :value="item.status" />
                </template>
              </v-list-item>
              <v-divider />
            </template>
          </v-list>
        </AppPagination>
      </v-col>
    </v-row>
    <StoreUserForm @store="select()" ref="storeUserForm" />
  </div>
</template>

<script setup>
definePageMeta({
  middleware: ["auth"],
  title: "Usuários",
});
const { $loading } = useNuxtApp();

const storeUserForm = ref(null);

const userStore = useUserStore();

const query = computed(() => userStore.query);

const users = computed(() => userStore.users);

onMounted(() => {
  userStore.$reset();

  select(1);
});

const select = async (page = null) => {
  $loading.start();
  if (page) {
    userStore.query.page = page;
  }

  await userStore
    .index()
    .then((response) => {
      $loading.finish();
    })
    .catch((error) => {
      $loading.finish();
    });
};

const handleNewUser = () => {
  storeUserForm.value.open();
};

const handleEditUser = (id) => {
  storeUserForm.value.open(id);
};
</script>

<style></style>