<?php
    $conn = new mysqli("localhost:3306", "root", "254425", "invenControlDB");
    if($conn->connect_error) die("connect error : ".$conn->error);

    $sql = "SELECT trans_customerName 
            FROM (SELECT trans_customerName, pro_supplierName, COUNT(*) AS maxbuy 
                  FROM  products, transactions
                  WHERE pro_productID = trans_productID
                  GROUP BY trans_customerName, pro_supplierName
                  ORDER BY trans_customerName, pro_supplierName) numofdupbuy
            GROUP BY trans_customerName
            HAVING MAX(maxbuy) >= ".$_POST["thiscount"];
    if($conn->query($sql) == FALSE) die("select error : ".$conn->error);

    echo "<table>
            <tr>
                <td>Product Name</td>
            <tr>";

    $result = $conn->query($sql);
    if($result->num_rows > 0){
        while($data = $result->fetch_assoc()){
            echo "<tr>
                    <td>".$data["pro_name"]."</td>
                  <tr>";
        }
    }
    echo "</table>";
?>