<template>
  <v-dialog v-model="dialog" max-width="1200px">
    <v-card>
      <v-card-text>
        <v-row>
          <v-col>
            <app-text-field label="Nome" v-model="form.product_name" />
          </v-col>
        </v-row>
        <v-row>
          <v-col>
            <app-text-field label="Quantidade" v-model="form.quantity" />
          </v-col>
          <v-col>
            <app-text-field label="Preço" v-model="form.price" />
          </v-col>
        </v-row>
      </v-card-text>
      <v-card-actions>
        <v-btn class="text-capitalize" color="disabled" @click="dialog = false">Cancelar</v-btn>
        <v-btn class="text-capitalize" color="primary" variant="elevated" @click="store()">Salvar</v-btn>
      </v-card-actions>
    </v-card>
  </v-dialog>
</template>

<script setup>

const orderForm = useOrderFormStore()

const dialog = ref(false)

const shouldCloseAfterStore = ref(false)

const add = () => {
  
  orderForm.resetOrderProductForm()

  shouldCloseAfterStore.value = false

  dialog.value = true


}

const edit = (index) => {
  
  orderForm.editOrderProduct(index)

  shouldCloseAfterStore.value = true

  dialog.value = true

}

const form = computed(() => {
  
  return orderForm.orderProductForm

})

const store = () => {
  
  orderForm.storeOrderProduct()

  if (shouldCloseAfterStore.value) {
    dialog.value = false
  }

}

defineExpose({
  add, edit
})


</script>
