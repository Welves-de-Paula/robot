<template>
  <v-text-field
    v-model="password"
    :append-icon="show1 ? 'mdi-eye' : 'mdi-eye-off'"
    :rules="[rules.required, rules.min]"
    :type="show1 ? 'text' : 'password'"
    name="input-10-1"
    label="Normal with hint text"
    hint="At least 8 characters"
    theme="dark"
    counter
    @click:append="show1 = !show1"
  />
</template>
<script>
export default {
  data() {
    return {
      show1: false,
      show2: true,
      password: "Password",
      rules: {
        required: (value) => !!value || "Required.",
        min: (v) => v.length >= 8 || "Min 8 characters",
        emailMatch: () => `The email and password you entered don't match`,
      },
    };
  },
};
</script>