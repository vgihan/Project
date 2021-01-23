<?php
    $conn = new mysqli("localhost:3306", "root", "254425", "invenControlDB");
    error_reporting(E_ALL);
    ini_set("display_errors", 1);

    $prevPage = $_SERVER["HTTP_REFERER"];

    if(is_uploaded_file($_FILES["csvFile"]["tmp_name"])){
        $fp = fopen($_FILES["csvFile"]["tmp_name"], "r");
        while($data = fgetcsv($fp, 2048, ",")){
            switch($data[0]){
                case "C":
                    $sql = "INSERT INTO customers
                            VALUES (\"".trim($data[1])."\", \"".trim($phone_num)."\", \"".trim($data[3])."\", \"".trim($data[4])."\")
                            ON DUPLICATE KEY UPDATE cus_name    = VALUES(cus_name),
                                                    cus_phone   = VALUES(cus_phone),
                                                    cus_address = VALUES(cus_address),
                                                    cus_gender  = VALUES(cus_gender);";
                    break;
                case "T":
                    $temp = explode("/", trim($data[4]));
                    $userdate = $temp[2]."-".$temp[1]."-".trim($temp[0]);
                    $replace_data = str_replace("\$", "", trim($data[3]));
                    $sql = "INSERT INTO transactions
                            VALUES (\"".trim($data[1])."\", \"".trim($data[2])."\", \"".trim($replace_data)."\", \"".$userdate."\", \"".trim($data[5])."\")
                            ON DUPLICATE KEY UPDATE trans_transactionNumber = VALUES(trans_transactionNumber),
                                                    trans_productID         = VALUES(trans_productID),
                                                    trans_price             = VALUES(trans_price),
                                                    trans_date              = VALUES(trans_date),
                                                    trans_customerName      = VALUES(trans_customerName);";
                    break;
                case "P":
                    foreach($data as $index){
                        ltrim($index);
                    }
                    $sql = "INSERT INTO products
                            VALUES (\"".trim($data[1])."\", \"".trim($data[2])."\", \"".trim($data[3])."\")
                            ON DUPLICATE KEY UPDATE pro_name         = VALUES(pro_name),
                                                    pro_productID    = VALUES(pro_productID),
                                                    pro_supplierName = VALUES(pro_supplierName)";
                    break;
            }
            if($conn->query($sql) == false) die("insert error : ".$conn->error);
        }
    }
    header("location:".$prevPage);
?>