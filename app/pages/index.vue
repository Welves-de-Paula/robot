<template>
  <v-container>
    <div>
      <v-row>
        <v-col>
          <v-btn @click="light">About</v-btn>
        </v-col>
      </v-row>
    </div>
  </v-container>
</template>

<script setup>
import axios from "axios";

const light = () => {
  //vai mandar um post true para 192.168.3.52

  // Define o endereço IP do ESP8266
  const espIP = "192.168.3.52"; // Substitua pelo endereço IP do seu ESP8266

  // Envia o pedido POST com o parâmetro "light" para o ESP8266
  axios
    .post(`http://${espIP}`, { light: true })
    .then((response) => {
      console.log(response.data);
    })
    .catch((error) => {
      console.log(error);
    });
};
</script>


