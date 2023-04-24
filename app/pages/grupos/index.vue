
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
          @click="handleNewGroup()"
        >
          Novo
        </app-btn>
      </v-col>
    </v-row>

    <v-row>
      <v-col>
        <AppPagination
          v-model="query.page"
          :data="groups"
          @update:modelValue="select()"
        >
          <v-list ref="list" @click:select="handleEditGroup(item.id)">
            <template v-for="(item, i) in groups.data" density="compact">
              <v-list-group>
                <template v-slot:activator="{ props }">
                  <v-list-item v-bind="props" :value="item.id">
                    <template v-slot:prepend>
                      <small class="mr-2">{{ item.code }}# </small>
                    </template>
                    <v-list-item-title v-text="item.name" />
                    <template v-slot:append>
                      <AppStatusChip :value="item.status" />
                    </template>
                  </v-list-item>
                  <v-divider />
                </template>

                <v-list-item
                  density="compact"
                  nav
                  v-for="(subgroup, i) in item.subgroups"
                  @click="handleEditSubGroup(subgroup.id)"
                >
                  <template v-slot:prepend>
                    <small class="mr-2">{{ subgroup.code }}# </small>
                  </template>
                  <v-list-item-title
                    class="text--small"
                    v-text="subgroup.name"
                  />
                  <template v-slot:append>
                    <AppStatusChip :value="subgroup.status" />
                  </template>
                </v-list-item>

                <v-list-item
                  density="compact"
                  nav
                  title="Adicionar Subgrupo"
                  prepend-icon="mdi-plus"
                  @click="handleNewSubGroup(item.id)"
                />
              </v-list-group>
            </template>
          </v-list>
        </AppPagination>
      </v-col>
    </v-row>
    <StoreGroupForm @store="select()" ref="groupForm" />
    <StoreSubGroupForm @store="select()" ref="subGroupForm" />
  </div>
</template>

<script setup>
definePageMeta({
  middleware: ["auth"],
  title: "Grupos",
});

const list = ref(null);

const { $loading } = useNuxtApp();

const groupForm = ref(null);

const subGroupForm = ref(null);

const groupStore = useGroupStore();

const query = computed(() => groupStore.query);

const groups = computed(() => groupStore.groups);

onMounted(() => {
  groupStore.$reset();

  select(1);
});

const select = async (page = null) => {
  $loading.start();

  if (page) {
    groupStore.query.page = page;
  }

  await groupStore.index();

  $loading.stop();
};

const handleNewGroup = () => {
  groupForm.value.open();
};

const handleNewSubGroup = (group_id) => {
  subGroupForm.value.group_id = group_id;

  subGroupForm.value.open();
};

const handleEditGroup = (id) => {
  groupForm.value.open(id);
};
const handleEditSubGroup = (subgroup_id) => {
  subGroupForm.value.open(subgroup_id);
};
</script>

<style></style>
