
import BlogFooter from '@/components/BlogFooter.vue';


import BlogHeader from '@/components/BlogHeader.vue';

<template>

    <BlogHeader/>

    <div id="grid">
        <div id="signup">
            <h3>注册账号</h3>
            <form>
                <div class="form-elem">
                    <span>账&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;号:</span>
                    <input v-model="signupName" type="text" placeholder="输入用户名">
                </div>
                <div class="form-elem">
                    <span>密&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;码:</span>
                    <input v-model="signupPwd" type="password" placeholder="输入密码">
                </div>
                <!-- 验证两次密码 -->
                <div class="form-elem">
                    <span>确认密码:</span>
                    <input v-model="checkPwd" type="password" placeholder="请再次输入">
                </div>
                <div class="form-elem">
                    <button v-on:click.prevent="signup">提交</button>
                </div>
            </form>
        </div>

        <div id="singin">
            <h3>登陆账号</h3>
            <form>
                <div class="form-elem">
                    <span>账号:</span>
                    <input v-model="signinName" type="text" placeholder="输入用户名">
                </div>

                <div class="form-elem">
                    <span>密码:</span>
                    <input v-model="signinPwd" type="password" placeholder="输入密码">
                </div>

                <div class="form-elem">
                    <button v-on:click.prevent="signin">登录</button>
                </div>
            </form>
        </div>
    </div>

    <BlogFooter/>

</template>

<script>
    import axios from 'axios';
    import BlogHeader from '@/components/BlogHeader.vue';
    import BlogFooter from '@/components/BlogFooter.vue';
    // eslint-disable-next-line vue/multi-word-component-names
    export default {
        // eslint-disable-next-line vue/multi-word-component-names
        name: 'Login',
        components: {BlogHeader, BlogFooter},
        data: function () {
            return {
                signupName: '',
                signupPwd: '',
                checkPwd: '',
                signupResponse: null,
                signinName: '',
                signinPwd: '',
            }
        },
        methods: {
            signup() {
                const that = this;
                if (this.signupPwd !== this.checkPwd) {
                    alert("两次输入的密码不一致,再检查一下吧")
                    return
                }
                axios
                    .post('/api/user/', {
                        username: this.signupName,
                        password: this.signupPwd,
                    })
                    .then(function (response) {
                        that.signupResponse = response.data;
                        alert('Welcome to cy\'s world');
                    })
                    .catch(function () {
                        alert("用户名已经存在了,换一个吧!");
                        // Handling Error here...
                        // https://github.com/axios/axios#handling-errors
                    });
            },
            signin() {
                const that = this;
                axios
                    .post('/api/token/', {
                        username: that.signinName,
                        password: that.signinPwd,
                    })
                    .then(function (response) {
                        const storage = localStorage;
                        // Date.parse(...) 返回1970年1月1日UTC以来的毫秒数
                        // Token 被设置为1天，因此这里加上86400000毫秒
                        const expiredTime = Date.parse(response.headers.date) + 86400000;
                        // 设置 localStorage
                        storage.setItem('access.myblog', response.data.access);
                        storage.setItem('refresh.myblog', response.data.refresh);
                        storage.setItem('expiredTime.myblog', expiredTime);
                        storage.setItem('username.myblog', that.signinName);
                        // 显示是否是管理员
                        axios
                            .get('/api/user/' + that.signinName + '/')
                            .then(function (response) {
                                storage.setItem('isSuperuser.myblog', response.data.is_superuser);
                                //路由跳转修改到这里
                                // 路由跳转
                                // 登录成功后回到博客首页
                                that.$router.push({name: 'Home'});
                            });
                    })
                    .catch(function () {
                        alert("用户名或密码错误,请再次尝试!")
                    })
                    // 读者自行补充错误处理
                    // .catch(...)
            },
        }
    }
</script>

<style scoped>
    #grid {
        display: grid;
        grid-template-columns: 1fr 1fr;
    }
    #signup {
        text-align: center;
    }
    #signin {
        text-align: center;
    }
    .form-elem {
        padding: 10px;
    }
    input {
        height: 25px;
        padding-left: 10px;
    }
    button {
        height: 35px;
        cursor: pointer;
        border: none;
        outline: none;
        background: gray;
        color: whitesmoke;
        border-radius: 5px;
        width: 60px;
    }
</style>