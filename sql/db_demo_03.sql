create table  t14(
    birthday date,
    job_time DATETIME,
    login_time TIMESTAMP
        NOT NULL DEFAULT CURRENT_TIMESTAMP #默认使用当前时间
        ON UPDATE CURRENT_TIMESTAMP        #自动更新到当前时间

);
select * from t14;
insert into t14(birthday, job_time) values ('2022-11-11','2022-11-11 10:10:10');

create table `emp`(
    id INT,
    `name` varchar(32),
    sex char(1),
    birthday date,
    entry_data datetime,
    job varchar(32),
    salary double,
    `resume` text
) charset utf8 collate utf8mb3_bin engine INNODB;

insert into `emp` values (100,'小妖怪','男','2000-11-11','2010-10-11 11:11:11','砍菜的',3000,'烧锅煮饭');
#表emp 添加一列 image 不能为null 在 resume 字段后面
alter table `emp` add image varchar(32) not null default '' after resume;
#显示表结构 可以查看表所有列
desc emp;
#修改job列 使其长度为60
alter table emp modify job varchar(60) not null default '';
#删除sex 列
alter table employee drop sex;
#修改表名
rename table  emp to employee;
desc employee;
#修改表的字符集为utf8
alter table employee character set utf8;
#修改列明name
alter table employee change `name` user_name varchar(32) not null default '';

alter table employee add sex char(1) not null default '男' after user_name;
#全表salary 值改为5000
UPDATE employee set salary=5000;

update employee set salary =5000 where user_name='小妖怪';
update employee set salary =salary+1000 where user_name='小妖怪';
insert into `employee` values (200,'老妖怪','男','2000-11-11','2010-10-11 11:11:11','砍菜的',3000,'烧锅煮饭');


select  * from employee;

create table `goods`(
    id int,
    goods_name varchar(10),
    price double
);

insert into `goods`(id,goods_name,price)
            values (10,'华为手机',2000);
#给所有字段添加数据 可以不写字段名
insert into `goods`
            values (10,'苹果手机',4000);
#一次添加多条数据
insert into `goods`(id,goods_name,price)
            values (10,'vivo手机',NULL),(10,'opp手机',NULL);
#列没有指定not null 那么当添加数据时 没有给定值 默认会给null
insert into `goods`(id,goods_name,price)
            values (10,'小米手机');#price 为null
desc `goods`;
select  * from goods;
#删除数据
delete from `goods` where goods_name='华为手机';
#删除表中所有数据
delete from goods;
#删除表
drop table goods;

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
#统计三门学科的总分
select `name`,(chinese+english+math) from student;

select `name`,(chinese+english+math+10) from student;
#列数据运算 as 取别名
select `name` as '名字',(chinese+english+math+10) as total_score from student;

#where 条件查询
select * from student where `name`='关羽';
select * from student where english>90;
select * from student where (chinese+english+math)>200;

select * from student where math>60 and id>1;
select * from student where (chinese+english+math)>200 and math <chinese and `name` like '关%';
select * from student where english>chinese;
#between 区间
select * from student where english>=80 and english<=90;
select * from student where english between 80 and 90;
#or 或者
select * from student where math=89 or math=90 or math=91;
select * from student where math IN (85,90,91);

select * from student where `name` like '关%' or `name` like '张%';
#order by 默认升序
select * from student order by math;
#desc 降序
select * from student order by math desc ;
#使用别名排序
select `name` as '名字',(chinese+english+math+10) as total_score from student order by total_score desc;

#count 统计 满足条件的有多少个 会排除有NULL的数据
select count(*) from student;
select count(*) from student where math>90;
select count(*) from student where(math+english+chinese)>200;

create table t15(
    `name` varchar(20)
);
insert into t15 values('tom'),('jack'),('mary'),(NULL);
select * from t15;

select count(`name`) from t15;
#统计数学总成绩
select sum(math) from student;
select sum(math) as math_total_score,sum(english),sum(chinese) from student;
#求平均值
select sum(chinese)/count(*) from student;

#avg求平均值
select avg(math) from student;

select max(math+english+chinese),min(math+english+chinese) from student;
select max(math),min(math) from student;

#group by 分组
#部门表
create table dept(
    deptno mediumint unsigned not null default 0,
    dname varchar(20) not null default '',
    loc varchar(13) not null default ''
);
insert into dept
    values (10,'accounting','new york'),(20,'research','dallas'),
           (30,'sales','chicago'),(40,'operations','boston');
select * from dept;

#员工表
create table emp(
    empno mediumint unsigned not null default 0,
    ename varchar(20) not null default '',
    job varchar(9) not null default '',
    mgr mediumint unsigned,
    hiredate date not null ,
    sal decimal(7,2) not null ,
    comm decimal(7,2),
    deptno mediumint unsigned not null default 0
);

INSERT INTO emp VALUES(7369, 'SMITH', 'CLERK', 7902, '1990-12-17', 800.00,NULL , 20),
                      (7499, 'ALLEN', 'SALESMAN', 7698, '1991-2-20', 1600.00, 300.00, 30),
                      (7521, 'WARD', 'SALESMAN', 7698, '1991-2-22', 1250.00, 500.00, 30),
                      (7566, 'JONES', 'MANAGER', 7839, '1991-4-2', 2975.00,NULL,20),
                      (7654, 'MARTIN', 'SALESMAN', 7698, '1991-9-28',1250.00,1400.00,30),
                      (7698, 'BLAKE','MANAGER', 7839,'1991-5-1', 2850.00,NULL,30),
                      (7782, 'CLARK','MANAGER', 7839, '1991-6-9',2450.00,NULL,10),
                      (7788, 'SCOTT','ANALYST',7566, '1997-4-19',3000.00,NULL,20),
                      (7839, 'KING','PRESIDENT',NULL,'1991-11-17',5000.00,NULL,10),
                      (7844, 'TURNER', 'SALESMAN',7698, '1991-9-8', 1500.00, NULL,30),
                      (7900, 'JAMES','CLERK',7698, '1991-12-3',950.00,NULL,30),
                      (7902, 'FORD', 'ANALYST',7566,'1991-12-3',3000.00, NULL,20),
                      (7934,'MILLER','CLERK',7782,'1992-1-23', 1300.00, NULL,10);

select * from emp;

#工资级别表
CREATE TABLE salgrade
(
    grade MEDIUMINT UNSIGNED NOT NULL DEFAULT 0,#工资级别
    losal DECIMAL(17,2)  NOT NULL,
    hisal DECIMAL(17,2)  NOT NULL
);

INSERT INTO salgrade VALUES (1,700,1200);
INSERT INTO salgrade VALUES (2,1201,1400);
INSERT INTO salgrade VALUES (3,1401,2000);
INSERT INTO salgrade VALUES (4,2001,3000);
INSERT INTO salgrade VALUES (5,3001,9999);

select * from salgrade;

#平均工资 最高工资 部门
select avg(sal),max(sal),deptno from emp group by deptno;
#平均工资 最低工资 部门 岗位 (每个部门每个岗位的平均工资)
select avg(sal),min(sal), deptno, job from emp group by deptno,job;
# group by 和 having 配合使
select avg(sal) as avgSal,deptno from emp group by deptno having avgSal<2000;

#返回字符串字符集
select charset(ename) from emp;

#连接字符串
select concat(ename, ' job is ', job) from emp;

#instr 返回substring 在string 中出现的位置
select instr('hanshunping','ping') from dual;

#ucase 转换成大写
select ucase(ename) from emp;
#lcase 转换为小写
select lcase(ename) from emp;
#left 取字符串左边2位
select left(ename,2) from emp;

#right 取字符串右边2位
select right(ename,2) from emp;

#返回string字节长度
select length('带带我') from emp;

#replace 字符串替换
select ename,replace(job,'MANAGER','经理') from emp;
#strcmp 比较2个字符串是否相等 0相等
select strcmp('hsp','hsp') from emp;
#取ename 1-2间的字符串
select substring(ename,1,2) from emp;

#去空格
select ltrim('   空格') from dual;
select rtrim('   空格   ') from dual;
select trim('   空格   ') from dual;

#substring 从1开
select concat(lcase(substring(ename,1,1)),substring(ename,2)) from emp;
select concat(lcase(left(ename,1)),substring(ename,2)) from emp;

#abs 取绝对值
select abs(-10) from dual;
#bin 十进制转二进制
select bin(10) from dual;
#ceiling 向上取整 2 比num大的最小整数
select ceiling(1.1) from dual;
#floor(number) 取比number小的最大整数
select floor(1.1) from dual;
#conv(8,10,2) 十进制的8 转为二进制输出
select conv(8,10,2) from dual;
#conv(8,10,2) 十六进制的8 转为二进制输出
select conv(8,16,2) from dual;
#format()保留小数位 四舍五入
select format(78.12345,2) from dual;
#least 返回最小值
select least(0,1,-10,4) from dual;
#mod 求余
select mod(10,3) from dual;
#rand 随机数 返回不同随机数
select rand() from dual;

select rand(3) from dual;
#rand[seed] 返回固定随机数
select rand(6) from dual;



