import { createApp } from 'vue'
import App from './App.vue'
import router from './router'

//防止其他用户通过url访问用户中心
router.beforeEach((to, from, next) => {
    console.log(to.path)
    console.log(from.path)
    if (to.meta.requireAuth) {
        console.log(to.params.username)
        console.log(localStorage.getItem('username.myblog'))
        if (localStorage.getItem('access.myblog') && to.params.username === localStorage.getItem('username.myblog')) {
            next();
        } else if (to.params.username !== localStorage.getItem('username.myblog')) {
            alert('请不要尝试使用url访问其他用户资料!')
            next({
                path: from.path
            })
        }
        else {
            if (to.path === '/login') {
                next();
            }
            else {
                alert('请先登录!')
                next({
                    path: '/login'
                })
            }
        }
    }   else {
        next();
    }
})

URLSearchParams.prototype.appendIfExists = function (key, value) {
    if (value !== null && value !== undefined) {
        this.append(key, value)
    }
};

createApp(App).use(router).mount('#app');