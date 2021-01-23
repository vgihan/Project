<?php
    $conn = new mysqli("localhost:3306", "root", "254425", "invenControlDB");
    if($conn->connect_error) die("connect error:".$conn->error);

    $prevPage = $_SERVER["HTTP_REFERER"];

    $sql = "CREATE TABLE IF NOT EXISTS customers(
                cus_name    VARCHAR(30) PRIMARY KEY,
                cus_phone   VARCHAR(30),
                cus_address VARCHAR(30),
                cus_gender  VARCHAR(10)
            )";
    if($conn->query($sql) == FALSE) die("C create table error: ".$conn->error);

    $sql = "CREATE TABLE IF NOT EXISTS transactions(
                trans_transactionNumber VARCHAR(30) PRIMARY KEY,
                trans_productID         VARCHAR(30),
                trans_price             REAL,
                trans_date              DATE,
                trans_customerName      VARCHAR(30)
            )";
    if($conn->query($sql) == FALSE) die("T create table error: ".$conn->error);

    $sql = "CREATE TABLE IF NOT EXISTS products(
                pro_name            VARCHAR(30),
                pro_productID       VARCHAR(30) PRIMARY KEY,
                pro_supplierName    VARCHAR(30)
            )";
    if($conn->query($sql) == FALSE) die("P create table error: ".$conn->error);

    switch($_POST["classType"]){
        case "customer":
            $temp = explode("/", $_POST["cus_phone"]);
            $phone_num = $temp[2]."-".$temp[1]."-".$temp[0];
            $sql = "INSERT INTO customers
                    VALUES (\"".$_POST["cus_name"]."\", \"".$phone_num."\", \"".$_POST["cus_address"]."\", \"".$_POST["cus_gender"]."\")
                    ON DUPLICATE KEY UPDATE cus_name    = VALUES(cus_name),
                                            cus_phone   = VALUES(cus_phone),
                                            cus_address = VALUES(cus_address),
                                            cus_gender  = VALUES(cus_gender);";
            break;
        case "transaction":
            str_replace("\$", "", $_POST["trans_price"]);
            $sql = "INSERT INTO transactions
                    VALUES (\"".$_POST["trans_transactionNumber"]."\", \"".$_POST["trans_productID"]."\", \"".$_POST["trans_price"]."\", \"".$_POST["trans_date"]."\", \"".$_POST["trans_customerName"]."\")
                    ON DUPLICATE KEY UPDATE trans_transactionNumber = VALUES(trans_transactionNumber),
                                            trans_productID         = VALUES(trans_productID),
                                            trans_price             = VALUES(trans_price),
                                            trans_date              = VALUES(trans_date),
                                            trans_customerName      = VALUES(trans_customerName);";
            break;
        case "product":
            $sql = "INSERT INTO products
                    VALUES (\"".$_POST["pro_name"]."\", \"".$_POST["pro_productID"]."\", \"".$_POST["pro_supplierName"]."\")
                    ON DUPLICATE KEY UPDATE pro_name         = VALUES(pro_name),
                                            pro_productID    = VALUES(pro_productID),
                                            pro_supplierName = VALUES(pro_supplierName)";
            break;
    }  
    if($conn->query($sql) == FALSE) die("insert error: ".$conn->error);

    header("location:".$prevPage);
?>