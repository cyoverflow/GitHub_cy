import { createWebHistory, createRouter } from "vue-router";
import Home from "@/views/Home.vue";
import ArticleDetail from "@/views/ArticleDetail.vue";
import Login from "@/views/Login.vue";
import UserCenter from "@/views/UserCenter.vue";
import ArticleCreate from "@/views/ArticleCreate.vue";
import ArticleEdit from "@/views/ArticleEdit.vue";

const routes = [
    {
        path: "/",
        name: "Home",
        component: Home,
    },
    {
        path: "/article/:id",
        name: "ArticleDetail",
        component: ArticleDetail
    },
    // 修改 Javascript 代码时
    // 不要忘记在同级元素后加上逗号
    // 否则将报错
    // 后面章节类似
    {
        path: "/login",
        name: "Login",
        component: Login,
    },
    {
        path: "/user/:username",
        name: "UserCenter",
        component: UserCenter,
        //防止其他用户通过url访问用户中心
        meta: { requireAuth: true },
    },
    {
        path: "/article/create",
        name: "ArticleCreate",
        component: ArticleCreate
    },
    {
        path: "/article/edit/:id",
        name: "ArticleEdit",
        component: ArticleEdit
    },
];

const router = createRouter({
    history: createWebHistory(),
    routes,
});

export default router;