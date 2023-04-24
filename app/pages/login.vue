<template>
  <div class="d-flex" style="height: 100%">
    <div class="flex-grow-1">
      <v-row theme="dark" class="row justify-center h-100">
        <v-col md="3">
          <div style="margin-top: 80px"></div>
          <h1 class="text-center mb-4">Bem vindo ;-) !!</h1>

          <v-row class="mt-6">
            <v-col md="12">
              <app-text-field
                color="primary"
                v-model="form.email"
                label="Usuário"
              />
            </v-col>
          </v-row>

          <v-row class="mt-3">
            <v-col md="12">
              <app-password-field
                color="primary"
                @keyup.enter.native="login"
                v-model="form.password"
                dark
                label="Senha"
                :name="'password'"
              />
            </v-col>
          </v-row>

          <v-row class="mt-10">
            <v-col class="text-center">
              <v-btn
                class="text-capitalize"
                @click="login()"
                block
                color="primary"
              >
                Login
              </v-btn>
            </v-col>
          </v-row>
        </v-col>
      </v-row>
    </div>
  </div>
</template>

<script setup>
import { useAuthStore } from "./../store/auth";

definePageMeta({
  layout: "auth",
});

const form = reactive({
  email: "admin@erp.com",
  password: "1234",
});

const router = useRouter();

const authStore = useAuthStore();

const login = async () => {
  await authStore.login(form).then((response) => {
    router.push("/");
  });
};
</script>