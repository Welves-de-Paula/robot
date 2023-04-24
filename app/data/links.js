export default
    [
        { type: "route", path: '/clientes', displayName: 'Clientes', permission: 'customersIndex', icon: "mdi-account" },
        { type: "route", path: '/pedidos', displayName: "Pedidos", permission: 'ordersIndex', icon: "mdi-cart" },
        { type: "route", path: '/produtos', displayName: 'Produtos', permission: 'productsIndex', icon: "mdi-package-variant" },
        { type: "route", path: "/regioes", displayName: 'Regiões', permission: 'regionsIndex', icon: "mdi-map-marker" },
        { type: "route", path: "/grupos", displayName: 'Grupos', permission: 'groupsIndex', icon: "mdi-sitemap" },
        { type: "route", path: "/usuarios", displayName: 'Usuários', permission: 'usersIndex', icon: 'mdi-account-group' },
        { type: "route", path: "/vendedores", displayName: 'Vendedores', permission: 'salesmenIndex', icon: 'mdi-account-tie' },

        // {
        //   type: "group",
        //   displayName: "Dashboards",
        //   icon: "mdi-view-dashboard",
        //   routes: [{ type: "route", route: "salesmanDashboard" }],
        // },
        // {
        //     type: "group",
        //     displayName: "Clientes",
        //     icon: "mdi-account",
        //     routes: [
        //         { type: "route", route: "customerIndex" },
        //         // { type: "route", route: "consultInstallmentIndex" },
        //     ],
        // },
        // {
        //   type: "group",
        //   displayName: "Produtos",
        //   icon: "mdi-package-variant-closed",
        //   routes: [
        //     { type: "route", route: "brandsIndex" },
        //     { type: "route", route: "productIndex" },
        //     { type: "route", route: "supplierIndex" },
        //     { type: "route", route: "categoriesIndex" },
        //     { type: "route", route: "purchaseIndex" },
        //     { type: "route", route: "stockTransferIndex" },
        //     { type: "route", route: "stockAuditIndex" },
        //   ],
        // },
        // {
        //   type: "group",
        //   displayName: "Vendas",
        //   icon: "mdi-cart",
        //   routes: [
        //     { type: "route", route: "salesIndex" },
        //     // { type: "route", route: "deliveryIndex" },
        //     { type: "route", route: "paymentTermsIndex" },
        //     { type: "route", route: "commissionIndex" },
        //   ],
        // },
        // {
        //   type: "group",
        //   displayName: "Logística",
        //   icon: "mdi-truck",
        //   routes: [
        //     {
        //       type: "route",
        //       route: "deliveryIndex",
        //     },
        //   ],
        // },
        // {
        //   type: "group",
        //   displayName: "Financeiro",
        //   icon: "mdi-currency-usd",
        //   routes: [
        //     { type: "route", route: "accountTransactionIndex" },
        //     { type: "route", route: "billPayableIndex" },
        //     { type: "route", route: "billReceivableIndex" },
        //     { type: "route", route: "cashFlowIndex" },
        //   ],
        // },
        // { type: "route", route: "professionalIndex" },
        // { type: "route", route: "employeeIndex" },
        // { type: "route", route: "rolesIndex" },
        // { type: "route", route: "companiesIndex" },
    ]