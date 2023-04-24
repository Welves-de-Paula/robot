<template>
  <div>
    <template v-if="getResults">
      <slot></slot>
      <v-pagination v-if="showPagination" v-model="currentPage" class="mt-3" :length="pageCount" rounded="circle" density="compact"></v-pagination>
    </template>
    <template v-else>
      <NoResults style="margin-top: 120px;"/>
    </template>
  </div>
</template>

<script setup>

const props = defineProps({
  modelValue : {},
  data: {
    type: Object,
    required: true,
  },
});

const emit = defineEmits(['update:modelValue']);

const getResults = computed(() => props.data.data.length > 0)

const pageCount = computed(() => props.data.last_page)

const showPagination = computed(() => props.data.last_page > 1)

const currentPage = computed({ 
  
  get : () => props.modelValue,
  
  set : (value) => emit('update:modelValue', value)
});

</script>