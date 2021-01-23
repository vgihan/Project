<?php
    $conn = new mysqli("localhost:80", "root", "254425", "invenControlDB");
    if($conn->connect_error) die("connect error : ".$conn->error);

    $sql = "SELECT pro_name, SUM(trans_price) AS price_sum FROM transactions, products
            WHERE trans_date > \'".$_POST["value_date"]."\'
            GROUP BY trans_productID
            ORDER BY price_sum DESC
            LIMIT ".$_POST["value_k"]." OFFSET 0";
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