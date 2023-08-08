#current_date() 当前日期
select current_date
from dual;
#current_time() 获取当前时间
select current_time
from dual;
#current_time() 获取当前时间戳
select current_timestamp
from dual;
#datediff 求2个日期相差多少天
select datediff('2011-11-11', '1990-01-01')
from dual;

select datediff(now(), '1989-05-27') / 365
from dual;
#1989-05-27+80年 -当前时间
select datediff(date_add('1989-05-27', INTERVAL 80 YEAR), now())
from dual;
#timediff 两个时间差
select timediff('10:11:11', '06:10;11')
from dual;

select now()
from dual;

create table mes
(
    id        int,
    content   varchar(30),
    send_time datetime
);
#current_timestamp() 返回当前的日期和时间
insert into mes
values (1, '北京新闻', current_timestamp());
#now()返回当前的日期和时间
insert into mes
values (1, '上海新闻', now());

select *
from mes;

#date()只获取日期 不获取时间
select id, content, date(send_time)
from mes;
#date_add(send_time,interval 10 minute) 当前时间 加上10分钟 大于当前时间
select *
from mes
where date_add(send_time, interval 10 minute) >= now();
select *
from mes
where date_sub(now(), interval 10 minute) <= send_time;

#获取年
select year(now())
from dual;
select month(now())
from dual;
select day(now())
from dual;
select month('2013-11-10')
from dual;
#unix_timestamp() 返回1970-1-1 到现在的秒数
select unix_timestamp() / (24 * 3600 * 365)
from dual;
#from_unixtime()将秒转换为 年月日
select from_unixtime(unix_timestamp(), '%Y-%m-%d')
from dual;

#查看用户@IP地址
select user()
from dual;

#查看当前使用数据库名称
select database();

#md5(str) 加密
select md5('password')
from dual;
#mysql 默认加密使用PASSWORD
select PASSWORD('123456')
from dual;

#md5 加密后长度都是32位
select length(md5('password'))
from dual;
drop table user;

create table user
(
    id     int,
    `name` varchar(32) not null default '',
    pwd    char(32)    not null default ''
);

insert into user
values (100, '关羽', md5('123456'));
insert into user
values (100, '张飞', PASSWORD('123456'));
select *
from user;
select *
from user
where name = '关羽'
  and pwd = md5('123456');
#db_day_01.user 表示数据库.表
select *
from db_01.user;
select *
from mysql.user;

#返回北京
select if(true, '北京', '上海');

select IFNULL(null, '北京', '上海')
from dual;

select case
           when true then 'jack' # 为真返回jack
           when false then 'tome'
           else 'mary' end;
select *from emp;

#comm如果为null 返回0.0
select ename, if(comm is null, 0.0, comm)
from emp;
#comm如果为null 返回0.0
select ename, ifnull(comm, 0.0)
from emp;

select ename,
       (select case
                   when job = 'clerk' then '职员'
                   when job = 'manager' then '经理'
                   when job = 'salesman' then '销售人员'
                   else job end) as job
from emp;
#日记查询 注意时间格式
select *
from emp
where hiredate > '1992-01-01';
# %表示0到多个任意字符 姓名是S开头的员工
select ename, sal
from emp
where ename like 'S%';
# _ 表示当个任意字符 ，查找第三个字符为大写O的员工
select ename, sal
from emp
where ename like '__O%';

select *
from emp
where mgr is null;

#查看表结构
desc emp;
#从低到高排序sal
select *
from emp
order by sal;
#desc 降序 deptno升序 撒了降序
select *
from emp
order by deptno, sal desc;
#limit 每页行数*（页数-1）,每页行数  从0+1行开始取 3行
select *
from emp
order by empno
limit 0,3;
select *
from emp
order by empno
limit 3,3;
select *
from emp
order by empno
limit 6,3;

select *
from emp
order by empno desc
limit 20,5;

#查询每种岗位的雇员总数 平均工资
select count(*), avg(sal), job
from emp
group by job;
#查询获得补助的员工 count(comm) 对NULL 数据可以过滤掉
select count(*), count(comm)
from emp;
#count(comm)对NULL 数据可以过滤掉
select count(*), count(*) - count(comm)
from emp;
select count(*), count(if(comm is null, 1, NULL))
from emp;

#distinct 去重
select count(distinct mgr)
from emp;

select max(sal) - min(sal)
from emp;

select deptno, avg(sal) as avg_sal
from emp
group by deptno #按部门分组
having avg_sal > 1000 #排除平均工资大于1000
order by avg_sal desc #按平均工资降序排列
limit 0,2;
#取前2条数据

#多表查询
select ename, sal, dname, emp.deptno
from emp,
     dept
where emp.deptno = dept.deptno;

select ename, sal, dname, emp.deptno
from emp,
     dept
where emp.deptno = dept.deptno
  AND emp.deptno = 10;

select *
from emp;
select *
from salgrade;

select ename, sal, grade
from emp,
     salgrade
where emp.sal between losal and hisal;
select ename, sal, dept.dname
from emp,
     dept
order by dept.deptno desc;
# 自连接 针对同一张表取别名查询
select worke.ename as '员工', boss.ename as '上级'
from emp worke,
     emp boss
where worke.mgr = boss.empno;

#子查询
select deptno
from emp
where ename = 'SMITH';
#单行子查询
select *
from emp
where deptno = (select deptno from emp where ename = 'SMITH');

#查出10号部门的工作岗位
select distinct job
from emp
where deptno = 10;

select ename, job, sal, deptno
from emp
where job in (select distinct job from emp where deptno = 10)
  and deptno != 10;

select database();

select ename, sal, deptno
from emp
where sal > all (select sal from emp where deptno = 30);

select ename, sal, deptno
from emp
where sal > (select max(sal) from emp where deptno = 30);

#>any 条件满足一个就可以
select ename, sal, deptno
from emp
where sal > any (select sal from emp where deptno = 30);

select ename, sal, deptno
from emp
where sal > (select min(sal) from emp where deptno = 30);

#多列子查询
select deptno, job
from emp
where ename = 'SMITH';
#和ALLEN 在同一个部门 切工作相同的员工
select *
from emp
where (deptno, job) = (select deptno, job from emp where ename = 'ALLEN')
  and ename != 'ALLEN';

select deptno, avg(sal)
from emp
group by deptno;
select *
from emp,
     (select deptno, avg(sal) as avg_sal from emp group by deptno) temp
where sal > temp.avg_sal
  and emp.deptno = temp.deptno;

select *
from emp,
     (select deptno, max(sal) max_sal from emp group by deptno) temp
where sal = max_sal
  and emp.deptno = temp.deptno;
#统计出每个部门的人员数量
select count(*), deptno
from emp
group by deptno;
select *
from dept;
select tmp.*, dname, loc
from dept,
     (select count(*), deptno from emp group by deptno) tmp
where tmp.deptno = dept.deptno;

create table my_table01
(
    id     int,
    `name` varchar(32),
    sal    double,
    job    varchar(32),
    deptno int
);
desc my_table01;
select *
from my_table01;

#复制表数据
#把emp表的数据复制到my_table01
insert into my_table01(id, `name`, sal, job, deptno)
select empno, ename, sal, job, deptno
from emp;

#自我复制插入 把查询的结果插入my_table01
insert into my_table01
select *
from my_table01;

#删除表的重复记录
#把emp表结构信息 复制到my_tab02
create table my_tab02 like emp;
desc my_tab02;

insert into my_tab02
select *
from emp;
#创建一个临时表
create table my_tmp like my_tab02;
#将my_tab02 去重的查询结果 插入到临时表
insert into my_tmp
select distinct *
from my_tab02;
# 清除my_tab02的表数据
delete
from my_tab02;
# 将my_tmp 表数据插入到 my_tab02
insert into my_tab02
select *
from my_tmp;
# 删除表my_tmp
drop table my_tmp;

select *
from my_tab02;
select *
from my_tmp;

#union all 合并2个查询结果 不去重
select ename, sal, job
from emp
where sal > 2500
union all
select ename, sal, job
from emp
where job = 'MANAGER';

#union  合并2个查询结果 去重数据
select ename, sal, job
from emp
where sal > 2500
union
select ename, sal, job
from emp
where job = 'MANAGER';

create table stu
(
    id     int,
    `name` varchar(32)
);
delete
from stu;
insert into stu
values (1, 'jack'),
       (2, 'tem'),
       (3, 'kity'),
       (4, 'nono');
select *
from stu;

create table exam
(
    id    int,
    grade int
);
insert into exam
values (1, 56),
       (2, 76),
       (11, 8);
select *
from exam;

select `name`, stu.id
from stu,
     exam
where stu.id = exam.id;

#左外连接 显示所有人的成绩 没有成绩也要显示该人的姓名和id
select `name`, stu.id, grade
from stu
         left join exam on stu.id = exam.id;
#右外连接 显示所有成绩 没有名字匹配显示未null
select `name`, stu.id, grade
from stu
         right join exam on stu.id = exam.id;

select dname, emp.ename, emp.job
from dept
         left join emp on dept.deptno = emp.deptno;

select dname, emp.ename, emp.job
from emp
         right join dept on dept.deptno = emp.deptno;

#primary key 主键列式不可以重复的
create table t17
(
    id     int primary key,
    `name` varchar(32),
    email  varchar(32)
);
create table t19
(
    id     int primary key,
    `name` varchar(32),
    email  varchar(32),
    primary key (id)
);
#复合主键
create table t18
(
    id     int,
    `name` varchar(32),
    email  varchar(32),
    primary key (id, `name`)#id和name合起来不相同
);

insert into t18
values (1, 'jack', 'jack@163.com');
values (1, 'tom', 'tom@163.com');
values (2, 'tom', 'tom@163.com');
desc t17;
#unique 表示值唯一 不可以重复
create table t19
(
    id     int unique,
    `name` varchar(32),
    email  varchar(32)
);

#外键
create table my_class
(
    id     int primary key,
    `name` varchar(32) not null default ''
);

create table my_stu
(
    id       int primary key,
    `name`   varchar(32) not null default '',
    class_id int,
    foreign key (class_id) references my_class (id)#指定外键关系
);
insert into my_class
values (100, 'java'),
       (200, 'web');
select *
from my_class;

insert into my_stu
values (1, 'tom', 100);
insert into my_stu
values (2, 'jack', 200);
insert into my_stu
values (3, 'dwb', 300);

select *
from my_stu;

# check
create table t32
(
    id     int primary key,
    `name` varchar(32),
    sex    varchar(6) check ( sex in ('man', 'woman') ),
    sal    double check ( sal > 1000 and sal < 2000 )
);

insert into t32
values (1, 'javk', 'mid', 1);
drop table goods;
drop table customer;

create table goods
(
    goods_id   int primary key,
    goods_name varchar(32)    not null default '',
    unitprice  decimal(10, 2) not null default 0.00 check ( unitprice between 0 and 999.99),
    category   int            not null default 0,
    provider   varchar(64)    not null default ''
);

create table customer
(
    customer_id int primary key,#客户编号
    `name`      varchar(32) not null default '',
    address varchar(32) not null default '',
    email varchar(32) not null default '',
    sex enum ('男','女') not null,#枚举
    card_id     varchar(18) unicode #身份证
);
create table purchase
(
    order_id    int primary key,
    customer_id int not null default 0,
    goods_id    int not null default 0,
    nums        int not null default 0,
    foreign key (goods_id) references goods (goods_id),#goods_id外键关系
    foreign key (customer_id) references customer (customer_id)#customer_id外键关系
);

#自增长 auto_increment
create table t24
(
    id     int primary key auto_increment,
    email  varchar(32) not null default '',
    `name` varchar(32) not null default ''
);

desc t24;
insert into t24
values (NULL, 'jack@qq.com', 'jack');
insert into t24
values (4, 'tom@qq.com', 'tom');
insert into t24
values (null, 'moo@qq.com', 'moo');
select *
from t24;

create table t25
(
    id int,
    `name` varchar(32) not null default ''
);
#查询表是否有索引
show index from t25;
show indexes from t25;
show keys from t25;

#添加索引
#添加主键索引
alter table t25 add primary key (id);#在id列添加主键索引
#添加唯一索引
create unique index id_index on t25(id);#唯一索引创建在id列
#添加普通索引
create index id_index on t25(id);
#添加普通索引方式二
alter table t25 add index id_index(id);

#删除索引
drop index id_index on t25;
#删除主键索引
alter table t25 drop primary key ;

create table t27
(
    id int,
    `name` varchar(32) not null default ''
);
#1. 开启事务
start transaction;
set autocommit =off;

#2.设置保存点a
savepoint a;
insert into t27 values (100,'tom');
select * from t27;

#2.设置保存点 b
savepoint b;
insert into t27 values (100,'jack');

#回退到保存点b;
rollback to b;
#回退到保存点a;
rollback to a;
#回退所有事务
rollback
