<template>
  <v-dialog v-model="dialog" max-width="1100" scrollable persistent>
    <v-card :title="title">
      <v-card-text>
        <v-container>
          <v-row>
            <v-col cols="12" sm="6" md="10">
              <AppTextField v-model="form.name" label="Nome" />
            </v-col>
            <v-col cols="12" sm="6" md="2">
              <AppTextField
                v-model="form.code"
                label="Código"
                readonly
                variant="outlined"
              />
            </v-col>

            <v-col cols="12" sm="6" md="4">
              <SearchGroup v-model="form.group" label="Grupo" />
            </v-col>

            <v-col cols="12" sm="6" md="4">
              <SearchSubgroup v-model="form.subgroup" label="Subgroup" />
            </v-col>

            <v-col cols="12" sm="6" md="4">
              <SearchBrand v-model="form.brand" label="Marca" />
            </v-col>

            <v-col cols="12" sm="6" md="4">
              <SearchUnities v-model="form.unity" label="Unidade" />
            </v-col>

            <v-col cols="12" sm="6" md="4">
              <SearchTaxClassification
                v-model="form.tax_classification"
                label="Classificação de Imposto"
              />
            </v-col>

            <!-- 
            <v-col cols="12">
              <v-divider />
            </v-col>

            <v-col cols="12" sm="6">
              <h3>Variantes de Produto</h3>
            </v-col> -->

            <template
              v-for="(variant, index) in form.product_variants"
              :key="index"
            >
              <!-- <h4>Variação {{ index + 1 }}</h4> -->

              <!-- <v-col cols="12">
                <AppTextField v-model="variant.name" label="Nome" />
              </v-col> -->

              <!-- <v-col cols="12" sm="6" md="4">
                <AppTextField v-model="variant.unit" label="Unidade" />
              </v-col> -->

              <v-col cols="12" sm="6" md="4">
                <AppTextField v-model="variant.gtin" label="GTIN" />
              </v-col>

              <v-col cols="12" sm="6" md="3">
                <AppTextField
                  v-model="variant.quantity_package"
                  label="Quantidade do Pacote"
                />
              </v-col>

              <v-col cols="12" sm="6" md="3">
                <AppTextField
                  v-model="variant.weight_net"
                  label="Peso Líquido"
                />
              </v-col>

              <v-col cols="12" sm="6" md="3">
                <AppTextField
                  v-model="variant.weight_gross"
                  label="Peso Bruto"
                />
              </v-col>

              <v-col cols="12" sm="6" md="3">
                <AppTextField
                  v-model="variant.weight_liters"
                  label="Peso em Litros"
                />
              </v-col>

              <v-col cols="12" sm="6" md="3">
                <AppTextField
                  v-model="variant.weight_conversion_factor"
                  label="Fator de Conversão de Peso"
                />
              </v-col>

              <v-col cols="12" sm="6" md="3">
                <AppTextField v-model="variant.commission" label="Comissão" />
              </v-col>

              <v-col cols="12" sm="6" md="3">
                <AppTextField
                  v-model="variant.price_raw_material"
                  label="Preço de Matéria-Prima"
                />
              </v-col>

              <v-col cols="12" sm="6" md="3">
                <AppTextField
                  v-model="variant.price_commission"
                  label="Preço com Comissão"
                />
              </v-col>

              <v-col cols="12" sm="6" md="3">
                <AppTextField v-model="variant.price" label="Preço" />
              </v-col>

              <!-- <v-col cols="12" sm="6" md="4">
                <AppTextField v-model="variant.stock" label="Estoque" />
              </v-col> -->

              <v-col cols="12" sm="6" md="3">
                <AppTextField
                  v-model="variant.stock_min"
                  label="Estoque Mínimo"
                />
              </v-col>

              <v-col cols="12" sm="6" md="3">
                <AppTextField
                  v-model="variant.stock_max"
                  label="Estoque Máximo"
                />
              </v-col>

              <v-col cols="12" sm="6" md="3">
                <AppTextField
                  v-model="variant.stock_location"
                  label="Localização do Estoque"
                />
              </v-col>
              <v-col cols="12" sm="6" md="3">
                <AppStatusSwitch v-model="form.status" />
              </v-col>
              <v-col cols="12">
                <v-textarea
                  v-model="form.notes"
                  label="Notas"
                  auto-grow
                  variant="outlined"
                  rows="1"
                  row-height="15"
                />
              </v-col>

              <!-- <v-col cols="12" sm="6" md="4">
                  <AppStatusSwitch v-model="variant.status" />
                </v-col> -->
            </template>
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
const emit = defineEmits(["store"]);

const dialog = ref(false);

import { defineEmits } from "vue";

const { $loading } = useNuxtApp();

const productForm = useProductFormStore();

const { form, title } = storeToRefs(productForm);

const open = async (id) => {
  productForm.$reset();
  $loading.start();

  if (id) {
    try {
      $loading.start();
      await productForm.show(id);

      $loading.finish();
    } catch (error) {
      console.log(error);
    } finally {
      $loading.finish();
    }
  }

  $loading.finish();
  dialog.value = true;
};

const handleSave = async () => {
  try {
    $loading.start();
    await productForm.save();
    emit("store");
    dialog.value = false;
    $loading.finish();
  } catch (error) {
    console.log(error);
  } finally {
    $loading.finish();
  }
};

defineExpose({
  open,
});
</script>
