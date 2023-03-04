<template>
    <!-- br 标签给页脚留出位置 -->
    <br><br><br>
    <div id="footer">
        <p>cymoonlight.xyz</p>
    </div>
    <div id="beian">
        <a href="https://beian.miit.gov.cn/" target="_blank">吉ICP备2023001114号</a>
    </div>
</template>
  
<script>
    export default{
        name: 'BlogFooter'
    }
</script>

<!-- "scoped" 使样式仅在当前组件生效 -->
<style scoped>
  #footer {
    font-size: large;
    position: fixed;
    left: 0;
    bottom: 9px;
    height: 60px;
    width: 100%;
    background: whitesmoke;
    text-align: center;
    font-weight: bold;
  }
  #beian {
    font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
    font-size: small;
    position: fixed;
    left: 0;
    bottom: 0;
    height: 22px;
    width: 100%;
    background: whitesmoke;
    text-align: center;
    font-weight: lighter;
  }

  a {
    color: rgb(0, 0, 0);
    text-decoration: none;
  }
</style>