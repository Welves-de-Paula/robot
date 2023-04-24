<template>
  <v-avatar size="150px">
    <v-img @click="handleAvatarClick()" :src="image" />
    <input
      class="d-none"
      ref="fileInput"
      type="file"
      @input="handleFileInput()"
    />
  </v-avatar>
</template>

<script>
import MediaMixin from "@/mixins/Media/MediaMixin";
export default {
  mixins: [MediaMixin],

  props: {
    uploadUrl: {},
    associate: {},
    media: {},
  },

  data: () => ({
    image: null,

    form: {
      avatar: null,
    },
  }),

  computed: {
    avatarPath() {
      return this.getImage(this.media);
    },
  },

  watch: {
    avatarPath(val) {
      console.log(val);
      this.image = val;
    },
  },

  created() {
    this.image = this.avatarPath;
  },

  methods: {
    handleAvatarClick() {
      this.$refs.fileInput.click();
    },

    handleFileInput() {
      let input = this.$refs.fileInput;
      let files = input.files;

      if (files && files[0]) {
        let reader = new FileReader();

        reader.onload = (e) => {
          this.image = e.target.result;
          this.form.avatar = files[0];
          this.storeMedia();
        };

        reader.readAsDataURL(files[0]);
      }
    },

    getFormData() {
      let form = new FormData();

      form.append("avatar", this.form.avatar);

      return form;
    },

    async storeMedia() {
      await this.$http
        .$post(`${this.uploadUrl}/${this.associate.id}`, this.getFormData())
        .then((response) => {
          this.$emit("store");
        })
        .catch((erro) => {});
    },
  },
};
</script>

<style></style>
