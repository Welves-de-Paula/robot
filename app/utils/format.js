import { format, parse, parseISO } from 'date-fns'
import { ptBR } from 'date-fns/locale'
import numeral from 'numeral'
import "numeral/locales/pt-br";

export default {

  // numeral(value, format = '0 a') {
  //   if (!value) {
  //     var value = 0
  //   }

  //   numeral.locale('pt-br');

  //   return numeral(parseFloat(value)).format(format)
  // },

  decimal(value, decimalPlaces = 2) {
    if (!value) {
      var value = 0
    }

    numeral.locale('pt-br');

    return numeral(parseFloat(value)).format('0,0.' + "0".repeat(decimalPlaces))

  },

  // extenso(value, mode) {

  //   let default_mode = mode ? mode : 'currency'
  //   if (value) {
  //     try {
  //       return formatExtenso(this.decimal(value), { mode: default_mode });
  //     } catch (e) {
  //       console.log("🚀 ~ file: format.js ~ line 36 ~ extenso ~ e", e)
  //       return value
  //     }
  //   }
  //   return null
  // },

  date(value, dateFormat = 'dd/MM/yyyy') {
    if (value) {
      try {

        if (format(parseISO(value), 'yyyy') === format(new Date(), 'yyyy')) {
          return format(parseISO(value), 'dd/MM')
        }

        return format(parseISO(value), dateFormat)

      } catch (e) {
        return value
      }
    }
  },

  dateBr(value, dateFormat = 'dd/MM/yyyy') {
    if (value) {
      try {
        return format(parseISO(value), dateFormat, { locale: ptBR })
      } catch (e) {
        return value
      }
    }
  },

  weekDay(value) {
    if (value) {
      try {
        return format(parseISO(value), 'EEEE', { locale: ptBR })
      } catch (e) {
        return value
      }
    }
  },


  // dateTimeBr(value, dateFormat = 'dd/MM/yyyy HH:mm:ss') {
  //   if (value) {
  //     try {
  //       return format(parseISO(value), dateFormat)
  //     } catch (e) {
  //       return value
  //     }
  //   }
  // },

  // time(value, timeFormat = 'HH:mm') {
  //   if (value) {
  //     try {
  //       return format(parse(value, 'HH:mm:ss', new Date()), timeFormat)
  //     } catch (e) {
  //       return value
  //     }
  //   }
  // },

  // dateRelative(date) {
  //   if (date) {
  //     try {
  //       let now = new Date();
  //       let diff = now - parseISO(date);
  //       let second = 1000,
  //         minute = second * 60,
  //         hour = minute * 60,
  //         day = hour * 24,
  //         week = day * 7;

  //       if (diff < 5 * minute) {
  //         return "Agora mesmo";
  //       }
  //       if (diff < 1 * hour) {
  //         return Math.floor(diff / minute) + " minutos atrás";
  //       }
  //       if (diff < 2 * hour) {
  //         return "1 hora atrás";
  //       }
  //       if (diff < 1 * day) {
  //         return Math.floor(diff / hour) + " horas atrás";
  //       }
  //       if (diff < 2 * day) {
  //         return "Ontem";
  //       }
  //       if (diff < 7 * day) {
  //         return Math.floor(diff / day) + " dias atrás";
  //       }
  //       if (diff < 30 * day) {
  //         return Math.floor(diff / week) + " semanas atrás";
  //       }
  //       if (diff < 365 * day) {
  //         return Math.floor(diff / day / 30) + " meses atrás";
  //       }

  //       return Math.floor(diff / day / 365) + " anos atrás";
  //     }
  //     catch (e) {
  //       return date
  //     }
  //   }
  // },

  // cpfCnpj() {

  // },

  // cnpj(value) {
  //   if (value) {
  //     return value.replace(/(\d{2})(\d{3})(\d{3})(\d{4})(\d{2})/g, "\$1.\$2.\$3\/\$4\-\$5")
  //   }
  //   return ''
  // },

  // cpf(value) {
  //   if (value) {
  //     return value.replace(/(\d{3})(\d{3})(\d{3})(\d{2})/g, "\$1.\$2.\$3-\$4")
  //   }
  //   return ''
  // },

  // rg(value) {
  //   if (value) {
  //     return value.replace(/(\d{2})(\d{3})(\d{3})(\d{1})/g, "\$1.\$2.\$3-\$4")
  //   }
  //   return ''
  // },

  // cep(value) {

  //   if (value) {
  //     return value.replace(/(\d{5})(\d{3})/g, "\$1-\$2")
  //   }
  //   return ''
  // },

  // phone(value) {

  //   if (value) {

  //     if (value.length == 11) {
  //       return value.replace(/(\d{2})(\d{1})(\d{4})(\d{4})/g, "(\$1) \$2 \$3-\$4")
  //     }
  //     if (value.length == 10) {
  //       return value.replace(/(\d{2})(\d{4})(\d{4})/g, "(\$1) \$2-\$3")
  //     }

  //     return ''

  //   }

  //   return null
  // },

  // moneyBr(value) {
  //   if (!value) {
  //     var value = 0
  //   }

  //   var parsedValue = parseFloat(value)
  //   var value = parsedValue.toFixed(2).split('.');

  //   value[0] = value[0].split(/(?=(?:...)*$)/).join('.');

  //   return 'R$ ' + value.join(',')
  //   // return value.join(',');
  // },


  // firstName(value, names = 1) {
  //   var arr = value.split(' ')
  //   var result = []

  //   for (let i = 0; i < names; i++) {
  //     result.push(arr[i]);
  //   }

  //   return result.join(' ')
  // },

  // bytesToMB(value) {

  //   if (value) {
  //     return (value / 1024 / 1024).toFixed(2) + ' MB'
  //   }
  //   return 0
  // },

  // bytesToSize(bytes, decimals = 2) {

  //   if (bytes == 0) return '0 Bytes';

  //   var k = 1024, dm = decimals < 0 ? 0 : decimals;

  //   var sizes = ['Bytes', 'KB', 'MB', 'GB'];

  //   var i = Math.floor(Math.log(bytes) / Math.log(k));

  //   return `${parseFloat((bytes / Math.pow(k, i)).toFixed(dm))} ${sizes[i]}`



  // }


}