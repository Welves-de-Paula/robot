<template>
  <div>
    <template v-for="(contact, index) in contacts" :key="index">
      <v-row>
        <v-col md="5">
          <app-text-field
            v-model="contact.contact"
            label="Telefone"
            @input="handleInput($event, index)"
          />
        </v-col>
        <v-col cols="5" md="5">
          <v-select
            v-model="contact.marker"
            label="Tipo"
            :items="markers"
            :item-title="'description'"
            :item-value="'value'"
          />
        </v-col>
        <v-col cols="1">
          <v-btn
            v-if="hasMoreThanOneRow"
            @click="removeRow(index)"
            size="small"
            variant="text"
            icon="mdi-window-close"
          />
        </v-col>
      </v-row>
    </template>
  </div>
</template>

<script setup>
import { ref, computed, watch, defineProps, defineEmits } from "vue";

const props = defineProps({
  modelValue: {},
});

const contacts = ref([
  {
    contact: null,
    marker: null,
  },
]);

const markers = ref([]);

const hasMoreThanOneRow = computed(() => {
  return contacts.value.length > 1;
});

const setContacts = (value) => {
  contacts.value = value;
};

const removeRow = (index) => {
  contacts.value.splice(index, 1);
};

const handleInput = (value, index) => {
  if (isLastRow(index) && value !== null) {
    addRow();
  }
};

const isLastRow = (index) => {
  return index + 1 == contacts.value.length;
};

const addRow = () => {
  contacts.value.push({
    contact: null,
    marker: null,
  });
};

watch(
  () => props.modelValue,
  (val) => {
    setContacts(val);
  }
);

const emits = defineEmits(["update:modelValue"]);

watch(
  contacts,
  (val) => {
    emits("update:modelValue", val);
  },
  { deep: true }
);

setContacts(props.modelValue);
</script>

<style>
</style>