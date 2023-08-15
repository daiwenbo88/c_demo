# mysql

#### docker 启动mysql

```shell
 docker run -itd --name mysql-docker -p 3306:3306 -e MYSQL_ROOT_PASSWORD=123456 mysql
 #-name mysql-docker：设置容器名称为mysql-docker，可自定义为任意名称。
 #-p 3306:3306：映射容器服务的 3306 端口到宿主机的 3306 端口，外部主机可以直接通过 宿主机ip:3306 访问   到 MySQL 的服务。
 #MYSQL_ROOT_PASSWORD=123456：设置 MySQL 服务 root 用户的密码。
docker restart mysql # 重新启动mysql
docker ps #查看运行的容器
docker ps -l # 查询所有的容器
```



**命令行连接到mysql**

```shell
#创建 mysql containers
docker run -itd --name mysql-docker -p 3306:3306 -e MYSQL_ROOT_PASSWORD=123456 mysql

docker ps
docker exec -it mysql-docker bash # name =mysql-docker 
mysql -uroot -p123456 #-u 用户 -p密码
SELECT @@tx_isolation; #查看数据库事务隔离具备
show databases；#查看数据库
show engines；#查看数据量引擎
use databases;#连接数据库
quit #退出
exit #退出bash

#备份数据库
mysqldump --default-character-set=utf8mb4 --single-transaction -u root -p123456 -B db_day_01 > /home/backup/db_day_02.sql

#备所有数据库
mysqldump --default-character-set=utf8mb4 --single-transaction -u root -p123456 --all-databases > /home/backup/db_day_02.sql

#恢复数据库
mysql -uroot -p123456 #-u 用户 -p密码
source /home/backup/db_day_02.sql
```

#### 创建数据库

```sql
#删除数据库
DROP DATABASE db_day_01
#数据库创建
CREATE DATABASE db_01
#创建一个使用utf-8字符集的数据库
CREATE DATABASE db_02 CHARACTER SET utf8
#创建一个使用utf8字符集,并带校对规则的数据库 utf8_bin区分大小写 默认 utf8_general_ai 不区分大小写
CREATE DATABASE db_03 CHARACTER SET utf8 COLLATE utf8_bin
```

#### 创建表

```sql
#指令创建表
#character set 字符集
#collate 数据库校对规则 (是否忽略大小写)
#engine 引擎
#charset gbk 指定字段编码集
create table `user`(
    id INT,
    `name` varchar(255) charset gbk,
    `password` varchar(255) charset gbk,
    `birthday` date
)
character set utf8 collate utf8_bin engine INNODB
```
#### 表数据类型

**数值类型：**
- 整形：`tinyint[1个字节]` `smallint[2个字节]`、`mediumit[3个字节]`、`int[4个字节]`、`bigint[8个字节]`
- 小数：`float[4]`、`double[8]`、`decimal[m,d][不确定]`
- 文本：`char[0-255字符]`、`varchar [0-65535字节]`、`text [0-2^16-1]`、`longtext[0-2^32-1]`
- 二进制：`blob[0-65535]`、`longblob[0-2^32-1]`
- 日期：`date[ymd]`、`time[时分秒]`、`datetime[ymd H:mm:ss]`、`timestamp[时间戳]`

```sql
#bit(m) m在1-64 
#num bit(8) m=8 表示一个字节 一个字节表示（0-255）
#显示按bit 显示
create table t05(num bit(8))

#decimal(m,n) m最大取值65, n代表后面的小数位 不够用0填充，m被省略默认是10，d被省略默认是0
create table t06(
                num1 float,
                num2 double,
                num3 decimal(30,20),
                )
insert into t06  values() 

char(4) //4个字符 空间定长  
varchar(4)// 最大放4个字符 空间大小根据内容来决定       
```
#### 创建表 修改表

```sql
create table 'emp'(
    id INT,
    `name` varchar(32),
    sex char(1),
    birthday date,
    entry_data datetime,
    job varchar(32),
    salary double,
    `resume` text
) charset utf8 collate utf8mb3_bin engine INNODB

insert into `emp` values (100,'小妖怪','男','2000-11-11','2010-10-11 11:11:11','砍菜的',3000,'烧锅煮饭');
#表emp 添加一列 image 不能为null 在 resume 字段后面
alter table `emp` add image varchar(32) not null default '' after resume;
#显示表结构 可以查看表所有列
desc emp;
#修改job列 使其长度为60
alter table emp modify job varchar(60) not null default '';
#删除sex 列
alter table emp drop sex;
#修改表名
rename table  emp to employee;
desc employee;
#修改表的字符集为utf8
alter table employee character set utf8;
#修改列明name
alter table employee change `name` user_name varchar(32) not null default '';
```

#### 插入数据

```sql
create table `goods`(
    id int,
    goods_name varchar(10),
    price double
);

insert into `goods`(id,goods_name,price)
            values (10,'华为手机',2000);
#给所有字段添加数据 可以不写字段名
insert into `goods`(id,goods_name,price)
            values (10,'苹果手机',4000);
#一次添加多条数据            
insert into `goods`(id,goods_name,price)
            values (10,'vivo手机',NULL),(10,'opp手机',NULL);
            
#列没有指定not null 那么当添加数据时 没有给定值 默认会给null
insert into `goods`(id,goods_name,price)
            values (10,'小米手机');#price 为null            
```
#### 修改数据

```sql
#全表salary 值改为5000
UPDATE employee set salary=5000;
update employee set salary =5000 where user_name='小妖怪';
```
#### 删除数据

```sql
#删除数据
delete from `goods` where goods_name='华为手机';
#删除表中所有记录
delete from goods;
#删除表
drop table goods;
```

#### 查询数据

```sql
create table student(
    id int not null default 1,
    name varchar(20) not null default '',
    chinese float not null default 0.0,
    english float not null default 0.0,
    math float not null default 0.0
);
insert into student(id, name, chinese, english, math)
        values(1,'张飞',67,98,56),(2,'关羽',88,98,90),(2,'宋江',87,78,77);
#查询所有表信息
select * from student;
#查询表中所有学生的姓名和对应的英语成绩
select `name`,english from student;
#distinct 去重
select  distinct english from student;

select distinct `name`,english from student;

#列数据运算 as 取别名
select `name` as '名字',(chinese+english+math+10) as total_score from student;

#where 查询条件
select * from student where `name`='关羽';
select * from student where english>90;
select * from student where (chinese+english+math)>200;

# and 并列
from student where math>60 and id>1;

select * from student where (chinese+english+math)>200 and math < student.chinese and `name` like '关%';

#between 区间
select * from student where english>=80 and english<=90;
select * from student where english between 80 and 90;

#or 或者
select * from student where math=89 or math=90 or math=91;
select * from student where math IN (85,90,91);

#like 模糊查询
select * from student where `name` like '关%' or `name` like '张%';

```
#### 排序

```sql
#order by 默认升序
select * from student order by math;
#desc 降序
select * from student order by math desc ;
#使用别名排序
select `name` as '名字',(chinese+english+math+10) as total_score from student order by total_score desc;

```

#### 统计

```sql
#count 个数 会排除有NULL的数据
select count(`name`) from t15;
#统计数学总成绩
select sum(math) from student;
select sum(math) as math_total_score,sum(english),sum(chinese) from student;
#求平均值
select sum(chinese)/count(*) from student;

#avg求平均值
select avg(math) from student;
#max 最大值 min最小值
select max(math+english+chinese),min(math+english+chinese) from student;
select max(math),min(math) from student;
```

#### 分组

```sql
#平均工资 最高工资 部门
select avg(sal),max(sal),deptno from emp group by deptno;
#平均工资 最低工资 部门 岗位 (每个部门每个岗位的平均工资)
select avg(sal),min(sal), deptno, job from emp group by deptno,job;
# group by 和 having 配合使
select avg(sal) as avgSal,deptno from emp group by deptno having avgSal<2000;
```