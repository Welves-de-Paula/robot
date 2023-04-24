<!-- src/components/RegionIndex.vue -->
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
          @click="handleNewRegion()"
        >
          Novo
        </app-btn>
      </v-col>
    </v-row>

    <v-row>
      <v-col>
        <AppPagination
          v-model="query.page"
          :data="regions"
          @update:modelValue="select()"
        >
          <v-list>
            <template v-for="(item, i) in regions.data" density="compact">
              <v-list-item @click="handleEditRegion(item.id)" :value="item.id">
                <template v-slot:prepend>
                  <small class="mr-2">{{ item.code }}# </small>
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
    <StoreRegionForm @store="select()" ref="regionForm" />
    <!-- {{ region.query }} -->
  </div>
</template>

<script setup>
definePageMeta({
  middleware: ["auth"],
  title: "Regiões",
});

const { $loading } = useNuxtApp();

const regionForm = ref(null);

const regionStore = useRegionStore();

const query = computed(() => regionStore.query);

const regions = computed(() => regionStore.regions);

onMounted(() => {
  regionStore.$reset();

  select(1);
});

const select = async (page = null) => {
  $loading.start();

  if (page) {
    regionStore.query.page = page;
  }

  await regionStore.index();

  $loading.stop();
};

const handleNewRegion = () => {
  regionForm.value.open();
};

const handleEditRegion = (id) => {
  regionForm.value.open(id);
};
</script>

<style></style>
