<template>
    <div v-for="article in info.results" v-bind:key="article.url" id="articles">
        <!-- 显示标题图 -->
        <div class="grid" :style="gridStyle(article)">
            <div class="image-container">
                <img :src="imageIfExists(article)" alt="" class="image">
            </div>


            <div>
                <div>
                    <!-- 增加显示分类 -->
                    <span v-if="article.category !== null" class="category">
                        {{ article.category.title }}
                    </span>
                    <span v-for="tag in article.tags" v-bind:key="tag" class="tag">
                        {{ tag }}
                    </span>
                </div>

                <div class="a-title-container">
                    <router-link :to="{ name: 'ArticleDetail', params: { id: article.id } }" class="article-title">
                        {{ article.title }}
                    </router-link>
                </div>

                <div>{{ formatted_time(article.created) }}</div>
            </div>
        </div>
    </div>
    <br><br><br>
    <div id="paginator">
        <span v-if="is_page_exists('previous')">
            <router-link :to="get_path('previous')">
                Prev
            </router-link>
        </span>
        <span v-else>
                Prev
        </span>
        <span class="current-page">
                {{ get_page_param('current') }}
        </span>
        <span v-if="is_page_exists('next')">
            <router-link :to="get_path('next')">
                Next
            </router-link>
        </span>
        <span v-else>
                Next
        </span>
    </div>
</template>

<script>
import { ref } from 'vue';
import { useRoute } from 'vue-router';
import getArticleData from '@/composables/getArticleData.js'
import pagination from '@/composables/pagination.js'

export default {

    name: 'ArticleList',

    //组合式API入口
    setup() {
        const info = ref('');
        //创建路由
        const route = useRoute();
        //获取文章的列表数据的方法
        getArticleData(info, route);

        const {
            is_page_exists,
            get_page_param,
            get_path
        } = pagination(info, route);

        return {
            info,
            //由于 setup 外不再关注 get_article_data() 方法，因此可以不用返回它了
            //get_article_data,
            is_page_exists,
            get_page_param,
            get_path,
        }
    },

    //旧代码的状态数据注释掉
    // data: function () {
    //     return {
    //         info: ''
    //     }
    // },

    // 也移动到setup()
    // mounted() {
    //     this.get_article_data()
    // },
    methods: {
        imageIfExists(article) {
            if (article.avatar) {
                return article.avatar.content
            }
        },
        gridStyle(article) {
            if (article.avatar) {
                return {
                    display: 'grid',
                    gridTemplateColumns: '1fr 4fr'
                }
            }
        },
        formatted_time: function (iso_date_string) {
            const date = new Date(iso_date_string);
            return date.toLocaleDateString()
        },
        //判断页面是否存在(组合式API)
        // is_page_exists(direction) {
        //     if (direction === 'next') {
        //         return this.info.next !== null
        //     }
        //     return this.info.previous !== null
        // },
        // 获取页码(组合式API)
        // get_page_param: function (direction) {
        //     try {
        //         let url_string;
        //         switch (direction) {
        //             case 'next':
        //                 url_string = this.info.next;
        //                 break;
        //             case 'previous':
        //                 url_string = this.info.previous;
        //                 break;
        //             default:
        //                 return this.$route.query.page
        //         }
        //         const url = new URL(url_string);
        //         return url.searchParams.get('page')
        //     }
        //     catch (err) {
        //         return
        //     }
        // },
        // 获取文章列表数据(更改为组合式API)
        // get_article_data: function () {
        //     let url = '/api/article';
        //     let params = new URLSearchParams();
        //     // 注意 appendIfExists 方法是原生没有的
        //     // 原生只有 append 方法，但此方法不能判断值是否存在
        //     params.appendIfExists('page', this.$route.query.page);
        //     params.appendIfExists('search', this.$route.query.search);

        //     const paramsString = params.toString();
        //     if (paramsString.charAt(0) !== '') {
        //         url += '/?' + paramsString
        //     }

        //     axios
        //         .get(url)
        //         .then(response => (this.info = response.data))
        // },
        //如果下一页的路径存在，那么则返回其带参数的路径，否则就返回无任何参数的首页路径(组合式API)
        // get_path: function (direction) {
        //     let url = '';

        //     try {
        //         switch (direction) {
        //             case 'next':
        //                 if (this.info.next !== undefined) {
        //                     url += (new URL(this.info.next)).search
        //                 }
        //                 break;
        //             case 'previous':
        //                 if (this.info.previous !== undefined) {
        //                     url += (new URL(this.info.previous)).search
        //                 }
        //                 break;
        //         }
        //     }
        //     catch { return url }

        //     return url
        // }
    }
    // 改写为组合式API(挪到setup())
    // watch: {
    //     //监听路由是否变化
    //     $route() {
    //         this.get_article_data()
    //     }
    // }
}
</script>

<!-- "scoped" 使样式仅在当前组件生效 -->
<style scoped>
.image {
    width: 300px;
    border-radius: 10px;
    box-shadow: darkslategrey 0 0 12px;
}

.image-container {
    width: 320px;
}

.grid {
    padding-bottom: 10px;
}

#articles {
    padding: 10px;
}

.article-title {
    font-size: xx-large;
    font-weight: bolder;
    color: black;
    text-decoration: none;
    padding: 5px 0 5px 0;
}

.tag {
    padding: 2px 5px 2px 5px;
    margin: 5px 5px 5px 0;
    font-family: Georgia, Arial, sans-serif;
    font-size: small;
    background-color: #4e4e4e;
    color: whitesmoke;
    border-radius: 5px;
}

#paginator {
    text-align: center;
    padding-bottom: 10px;
    font-weight: bold;
}

a {
    color: black;
    text-decoration: none;
}

.current-page {
    font-size: x-large;
    font-weight: bold;
    padding-left: 10px;
    padding-right: 10px;
}

.category {
    padding: 5px 10px 3px 10px;
    margin: 5px 5px 5px 0;
    font-family: Georgia, Arial, sans-serif;
    font-size: small;
    font-weight: bold;
    background-image: linear-gradient(120deg, #f6d365 0%, #fda085 100%);
    color: rgb(131, 63, 63);
    border-radius: 15px;
}
</style>