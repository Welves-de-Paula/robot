<template>
  <div class="d-flex" style="height: 100%">
    <div class="flex-grow-1" style="background-color: #171d34">
      <v-row class="row justify-center h-100">
        <v-col md="2">
          <div style="margin-top: 80px">
            <!-- <img
                class="mb-4"
                width="150px"
                src="https://erp.apexsoftware.com.br/img/logo_light.svg"
              /> -->
          </div>
          <h1 class="text-center mb-4 teal--text text-white">
            Bem vindo ;-) !!
          </h1>

          <v-row class="mt-6">
            <v-col md="12">
              <app-text-field
                color="primary"
                v-model="form.email"
                theme="dark"
                label="Usuário"
              />
            </v-col>
          </v-row>

          <v-row class="mt-3">
            <v-col md="12">
              <!-- <app-password-field
                color="primary"
                @keyup.enter.native="login"
                v-model="form.password"
                dark
                label="Senha"
                :name="'password'"
              /> -->
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
<script>
export default {
  data() {
    return {
      form: {
        email: null,
        password: null,
      },
    };
  },

  methods: {
    login() {
      this.$auth
        .login(this.form)
        .then(async (response) => {
          await this.$auth.getLoggedUser();
          this.$router.push("/");
        })
        .catch((error) => {
          this.$store.commit("app/alert", "Usuário ou senha inválidos");
        });
    },
  },
};
</script>

 