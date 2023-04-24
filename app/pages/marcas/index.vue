
<template>
  <div>

    <v-row>
      <v-col cols="12" md="4">
        <AppSearchField v-model="query.search" @update:modelValue="select(1)" />
      </v-col>
      <v-col class="text-right d-md-block d-none" cols="8">
        <app-btn class="text-capitalize" bottom color="primary" @click="handleNew()">
          Novo
        </app-btn>
      </v-col>
    </v-row>

    <v-row>
      <v-col>
        <AppPagination v-model="query.page" :data="brands" @update:modelValue="select()">
          <v-list ref="list" >
            <template v-for="(item, i) in brands.data" density="compact">
              <v-list-group @click="handleEdit(item.id)">
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
              </v-list-group>
            </template>
          </v-list>
        </AppPagination>
      </v-col>
    </v-row>
    <StoreBrandForm ref="storeBrandForm" @store="select(query.page)" />
  </div>
</template>

<script setup>

definePageMeta({
  middleware: ["auth"],
  title: "Marcas",
});

const list = ref(null);

const { $loading } = useNuxtApp();

const storeBrandForm = ref(null);

const brandStore = useBrandStore();


const query = computed(() => brandStore.query);

const brands = computed(() => brandStore.brands);


onMounted(() => {
  select(1);
});


const select = async (page = null) => {
  $loading.start();

  if (page) {
    brandStore.query.page = page;
  }

  await brandStore.index();

  $loading.stop();
};

const handleNew = () => {
  storeBrandForm.value.open();
};

const handleEdit = (id) => {
  
  storeBrandForm.value.open(id);

};

</script>

<style></style>
