<template>
  <div>
    <AppTextField
      :label="label"
      v-if="selectedValue"
      :model-value="selectedValue.name"
      @click="open()"
      @keypress="open()"
      @click:clear="clear()"
      readonly
      clearable
      prepend-inner-icon="mdi-magnify"
    />

    <div v-else id="acc" @click.stop="open()">
      <AppTextField
        :label="label"
        readonly
        @click="open()"
        prepend-inner-icon="mdi-magnify"
      />
    </div>

    <v-dialog v-model="dialog" scrollable max-width="500px">
      <v-card :title="`Selecionar ${label}`">
        <!-- <v-card-title>
          <div class="d-flex justify-space-between" style="width: 100%">
            <div>
              <span>Selecionar {{ label }}</span>
            </div>
          </div>
        </v-card-title> -->
        <v-card-subtitle class="px-5">
          <AppSearchField
            v-model="query.search"
            class="mt-2"
            density="compact"
            @update:modelValue="select()"
          />
        </v-card-subtitle>
        <v-card-text>
          <AppPagination
            v-model="query.page"
            :data="products"
            @update:modelValue="select()"
          >
            <v-list :lines="false">
              <v-list-item
                v-for="(product, i) in products.data"
                active-color="primary"
                @click="
                  setSelectedValue({
                    id: product.id,
                    code: product.code,
                    name: product.name,
                    product_variant: product.product_variant,
                  })
                "
                :value="product"
                :key="i"
              >
                <template v-slot:prepend>
                  <small class="mr-2">{{ product.code }}# </small>
                </template>
                <v-list-item-title v-text="product.name" />
              </v-list-item>
            </v-list>
          </AppPagination>
        </v-card-text>
        <v-card-actions>
          <v-btn text @click="dialog = false">Voltar</v-btn>
        </v-card-actions>
      </v-card>
    </v-dialog>
  </div>
</template>

<script setup>
const props = defineProps({
  label: {
    type: String,
    default: "",
  },
  modelValue: {
    type: Object,
    default: null,
  },
});
const { $loading } = useNuxtApp();

const emit = defineEmits(["update:modelValue", "input"]);

const productStore = useProductStore();

const dialog = ref(false);

const isLoading = ref(false);

const selectedValue = ref(null);

const query = computed(() => productStore.query);

const products = computed(() => productStore.products);
watch(
  () => props.modelValue,
  (newValue) => (selectedValue.value = newValue)
);

onMounted(() => (selectedValue.value = props.modelValue));

const open = () => {
  dialog.value = true;
  select();
};

const select = async () => {
  try {
    $loading.start();
    await productStore.index();
  } catch (error) {
    console.log(error);
  } finally {
    $loading.finish();
  }
};

const setSelectedValue = (product) => {
  selectedValue.value = product;

  emit("update:modelValue", product);
  emit("input");

  dialog.value = false;
};

const clear = () => {
  selectedValue.value = null;
  emit("update:modelValue", null);
  emit("input");
};

defineExpose({
  open,
});
</script>

<style lang="sass" >
</style>