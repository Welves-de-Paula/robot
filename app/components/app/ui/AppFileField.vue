<template>
  <div>
    <v-btn
      class="text-capitalize"
      @click="handleAddFile()"
      :color="color"
      v-if="!hasFile"
      :prepend-icon="icon"
    >
      <span v-if="showBtnText">{{ btnText }}</span>
    </v-btn>

    <v-btn
      v-if="hasFile"
      append-icon="mdi-delete"
      @click="clear()"
      rounded="0"
      :color="color"
      variant="outlined"
    >
      {{ btnText }}
      <template v-slot:append>
        <v-icon color="error" />
      </template>
    </v-btn>

    <v-file-input
      v-model="file"
      class="d-none"
      ref="fileInput"
      @update:modelValue="send()"
      :accept="props.accept"
    />
  </div>
</template>
<script setup>
const { $http, $loading } = useNuxtApp();

const emit = defineEmits(["input"]);

const props = defineProps({
  accept: {
    type: String,
    default: "*",
  },

  label: {
    type: String,
    default: "Abrir arquivo",
  },

  icon: {
    type: String,
    default: "mdi-upload",
  },

  showBtnText: {
    type: Boolean,
    default: true,
  },

  url: {
    type: String,
    default: "",
    required: true,
  },

  color: {
    type: String,
    default: "primary",
  },

  fileAlias: {
    type: String,
    default: "file",
  },
});

const file = ref(null);

const fileInput = ref(null);

const hasFile = computed(() => {
  return file.value && file.value[0] ? true : false;
});

const btnText = computed(() => {
  return hasFile && file.value !== null ? file.value[0].name : props.label;
});

const handleAddFile = () => {
  fileInput.value.click();
};

const clear = () => {
  fileInput.value = null;

  emit("input", null);
};

const send = async () => {
  if (hasFile) {
    $loading.start();
    let form = new FormData();
    form.append(props.fileAlias, file.value[0], props.fileAlias);
    await $http
      .$post(props.url, form)
      .then((response) => {
        emit("input", response);
        $loading.finish();
      })
      .catch((error) => {
        if (!props.url) {
          console.log("cade a url pra onde vai o arquivo ? (º~º) ");
        }
        clear();
        $loading.finish();
      });
  }
};
</script>