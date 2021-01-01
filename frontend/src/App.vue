<template>
  <el-card id="app">
      <header>水浊度检测系统</header>
      <el-row :gutter="20" id="row">
        <el-col :span="15">
          <div class="grid-content bg-purple">
            <el-card class="box-card" id="water_box">
            <div class="water_dirty">
              <el-progress type="dashboard" :percentage="parseInt(percentage)" :color="colors"></el-progress>
              <div id="reveal">水浊度: {{water_dirty}} NTU</div>
              <highcharts :options="chartOptions"></highcharts>
            </div>
            </el-card>
          </div>
        </el-col>
        <el-col :span="9">
          <div class="grid-content bg-purple">  
            <el-card class="box-card" id="switch_box">
              <div class="sw_switch">
                <div class="sw">水位检测开关：</div>
                  <el-tooltip :content="'Switch value: ' + switch_state" placement="top">
                    <el-switch
                      v-model="switch_view"
                      disabled>
                    </el-switch>
                  </el-tooltip>
                </div>
              </el-card>
              <el-divider></el-divider>
              <el-card class="box-card" id="info_box">
                <div class="zhibiao"><b>水浊度：</b>{{water_dirty}} NTU</div>
                  <div class="sw-info" v-if="switch_state=='1'">
                    <div v-if="sw_state=='0'" id="sw"><b>危险水位：</b>未达到</div>
                    <div color="red" v-else><b>危险水位：</b><span style="color:red">已达危险水位！！！</span></div>
                  </div>
                  <div class="sw-info-no" v-else style="color:red">未开启水位检测！</div>
              </el-card>
          </div>
        </el-col>
      </el-row>
  </el-card>
</template>

<script>
import axios from 'axios'
import highcharts from 'highcharts'
export default {
  name: 'App',
  data: function(){
    return{
      water_vol: "3.5",
      water_dirty:"5200",
      switch_state: "1",
      switch_view: true,
      sw_state: "0",
      chartCount: 0,
      percentage: 10,
        colors: [
          {color: '#f56c6c', percentage: 20},
          {color: '#e6a23c', percentage: 40},
          {color: '#5cb87a', percentage: 60},
          {color: '#1989fa', percentage: 80},
          {color: '#6f7ad3', percentage: 100}
        ],
      timer: '',
      chartOptions: {
        chart:{
          type:'spline',
        },
        series: [{
          data: []
        }],
        xAxis: {
          title: {
            text: "时间"
          },
          categories:[]
        },
        yAxis: {
          title: {
            text: "浊度(NTU)"
          }
        },
        title: {
          text: '浊度实时变化',
        },   
      }
    }
  },
  methods:{
    getData(){
      axios.get('http://niziqi.top:5000/sensorData')
      .then((res)=>{
        this.water_vol = res.data[0].water
        this.percentage = this.water_vol * 20 
        this.switch_state = res.data[0].switch
        this.sw_state = res.data[0].state
        this.water_dirty = -1120.4 * this.water_vol * this.water_vol + 5742.3 * this.water_vol - 4352.9
        this.drawChart()
        this.setSwitch()
      })
    },
    drawChart(){
      console.log(this)
      this.chartCount += 1
      this.chartOptions.series[0].data.push(this.water_vol)
      let nowDate = new Date()
      nowDate = nowDate.getHours() + ':' + nowDate.getMinutes()
      this.chartOptions.xAxis.categories.push(nowDate)
      if(this.chartCount>15){
          this.chartOptions.series[0].data.shift()
          this.chartOptions.xAxis.categories.shift()
          let nowDate = new Date()
          nowDate = nowDate.getHours() + ':' + nowDate.getMinutes()
          this.chartOptions.xAxis.categories.push(nowDate)
        }
    },
    setSwitch(){
      if(this.switch_state=='1'){
        this.switch_view=true
      }else{
        this.switch_view=false
      }
    }
  },
  mounted() {
    this.timer = setInterval(this.getData,1000);
  },
}
</script>

<style>
* {
  margin: 0;
  padding: 0;
}
.grid-content {
  height: 800px;
}
header {
  font-size: 40px;
  font-family: Avenir, Helvetica, Arial, sans-serif;
  margin-bottom: 40px;
  margin-top: 80px;
  margin-left: 170px;
  color: #11457B
}
body {
  display: flex;
  justify-content: center;
  align-items: center;
}
#row {
  width: 80vw;
}
#app {
  display: flex;
  margin-top: 50px;
  width: 90vw;
  height: 90vh;
  align-items: center;
  flex-wrap: nowrap;
  /* justify-content: center; */
  flex-direction: row;
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
}
#reveal {
  font-size: 18px;
  margin-bottom: 35px;
}
#water_box {
  width: 700px;
  height: 700px;
  margin-left: 200px;
}
.water_dirty {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  margin-top: 50px;
}
#switch_box {
  height: 200px;
  display: flex;
  margin-bottom: 60px;
  flex-direction: row;
  align-items: center;
  justify-content: center;
}
.sw {
  margin-bottom: 30px;
  font-size: 20px;
}
#info_box {
  display: flex;
  margin-top: 50px;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  font-size: 20px;
  height: 300px;
}
.zhibiao {
  margin-bottom: 50px;
}
</style>
