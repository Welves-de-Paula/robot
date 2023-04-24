<template>
  <v-navigation-drawer app dark floating color="#171D34" theme="dark">
    <v-sheet color="#171D34" class="pa-4">
      <!-- <b>Erp - Casas das Tintas</b> -->
      <div class="mt-2">
        <!-- <v-avatar class="mr-2" size="50"> -->
        <v-img
          src="https://tintasantares.com.br/wp-content/uploads/2022/07/logo-site.png"
        />
        <!-- <AppImg @click="changePassword()" :src="employee.media[0]" /> -->
        <!-- </v-avatar> -->
        <!-- <small> @{{ this.$store.state.auth.user.user.short_name }} </small> -->
      </div>
    </v-sheet>

    <v-divider />
    <v-list :lines="false" density="compact" nav theme="dark">
      <template v-for="(link, index) in links">
        <template v-if="link.type == 'route'">
          <v-list-item
            :to="link.path"
            :prepend-icon="link.icon"
            :title="link.displayName"
          />
        </template>
      </template>
    </v-list>

    <!-- <v-list-item-group>
        <template v-for="(link, index) in links">
          <template v-if="link.type == 'route'">
            <v-list-item
              v-if="hasPermission(link.route)"
              link
              :to="getRoutePath(link.route)"
              router
              exact
              :key="index"
            >
              <v-list-item-action>
                <v-icon>{{  }}</v-icon>
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title>{{
                }}</v-list-item-title>
              </v-list-item-content>
            </v-list-item>
          </template>

          <template v-if="link.type == 'group'">
            <v-list-group :key="index" v-if="hasGroupPermission(link.routes)">
              <template v-slot:activator>
                <v-list-item-action>
                  <v-icon>{{ link.icon }}</v-icon>
                </v-list-item-action>
                <v-list-item-content>
                  <v-list-item-title>{{ link.displayName }}</v-list-item-title>
                </v-list-item-content>
              </template>

              <template v-for="(subLink, index) in link.routes">
                <v-list-item
                  v-if="hasPermission(subLink.route)"
                  :to="getRoutePath(subLink.route)"
                  router
                  link
                  exact
                  :key="index"
                >
                  <v-list-item-action></v-list-item-action>
                  <v-list-item-content>
                    <v-list-item-title>{{
                      getRouteMeta(subLink.route).displayName
                    }}</v-list-item-title>
                  </v-list-item-content>
                </v-list-item>
              </template>
            </v-list-group> -->

    <template v-slot:append>
      <div class="pa-2">
        <v-btn @click="logout()" block> Logout </v-btn>
      </div>
    </template>
  </v-navigation-drawer>
</template>
<script setup>
import links from "@/data/links";

const router = useRouter();

const authStore = useAuthStore();

const logout = async () => {
  await authStore.logout().then((response) => {
    router.push("/login");
  });
};
</script>

<style>
</style>