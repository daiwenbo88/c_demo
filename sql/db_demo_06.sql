

#myisam 存储引擎
create table t28(
    id int,
    `name` varchar(32)) engine MYISAM;
# 1.添加速度快 2. 不支持外键和事务 3. 支持表级锁
start transaction;
savepoint t1;
insert into  t28 value (1,'jack');
select * from t28;
rollback to t1;

#memory 存储引擎
#1. 数据存储在内存中2 执行速度快 3. 默认支持索引（hash 表）
# 4. 关闭数据库 数据丢失 但是表结构还在
create table t29(
                    id int,
                    `name` varchar(32)) engine memory;
desc t29;
insert into t29 values(1,'tom'),(2,'jack'),(3,'dwb');
select  * from t29;

#修改存储引擎
alter table `t29` engine = innodb;
select TIMESTAMPDIFF(SECOND, '1970-1-1 8:0:0', NOW());
